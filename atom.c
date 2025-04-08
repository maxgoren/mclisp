#include <stdio.h>
#include <string.h>
#include "atom.h"
#include "list.h"
#include "primitives.h"
#include "specialform.h"

GC* collector;

enum AtomType typeOf(Atom* value) {
    return value != NULL ? value->type:AS_NIL;
}

bool compareValue(Atom* lhs, Atom* rhs) {
    if (lhs->type != rhs->type)
        return false;
    switch (lhs->type) {
        case AS_BOOL: return lhs->boolval == rhs->boolval;
        case AS_NUM: return lhs->intval == rhs->intval;
        case AS_BINDING: return compareBinding(lhs->bindval, rhs->bindval);
        case AS_FUNCTION: return false;
        case AS_LIST: return compareList(lhs->listval, rhs->listval);
        case AS_SYMBOL: return compareString(lhs->stringval, rhs->stringval);
        default: break;
    };
    return false;
}

bool compareString(String* lhs, String* rhs) {
    int i = 0;
    int j = 0;
    while (i < lhs->len && j < rhs->len) {
        if (lhs->data[i] != rhs->data[j])
            return false;
        i++; j++;
    }
    return  (i == lhs->len && j == rhs->len);
}

bool compareBinding(Binding* lhs, Binding* rhs) {
    return compareValue(lhs->symbol, rhs->symbol);
}

bool compareList(List* lhs, List* rhs) {
    listnode* lit = lhs->head;
    listnode* rit = rhs->head;
    while (lit != NULL && rit != NULL) {
        if (!compareValue(lit->info, rit->info))
            return false;
        lit = lit->next;
        rit = rit->next;
    }
    return lit == NULL && rit == NULL;
}

String* makeString(char* str, int len) {
    String* ns = malloc(sizeof(String));
    ns->data = malloc(sizeof(char) * len+1);
    int i;
    for (i = 0; str[i] && i <= len; i++) {
        ns->data[i] = str[i];
    }
    ns->data[i] = '\0';
    ns->len = i;
    return ns;
}

Atom* makeStringVal(String* str) {
    Atom* nv = malloc(sizeof(Atom));
    nv->type = AS_SYMBOL;
    nv->stringval = str;
    registerObject(collector, nv);
    return nv;
}

Atom* emptyList() {
    return makeListVal(createList());
}

Atom* makeIntVal(int value) {
    Atom* nv = malloc(sizeof(Atom));
    nv->intval = value;
    nv->type = AS_NUM;
    registerObject(collector, nv);
    return nv;
}

Atom* makeBoolVal(bool value) {
    Atom* nv = malloc(sizeof(Atom));
    nv->boolval = value;
    nv->type = AS_BOOL;
    registerObject(collector, nv);
    return nv;
}

Atom* makeListVal(List* list) {
    Atom* nv = malloc(sizeof(Atom));
    nv->type = AS_LIST;
    nv->listval = list;
    registerObject(collector, nv);
    return nv;
}

Binding* makeBinding(Atom* symbol, Atom* value) {
    Binding* binding = malloc(sizeof(Binding));
    binding->symbol = symbol;
    binding->value = value;
    return binding;
}

Atom* makeBindingVal(Binding* bind) {
    Atom* nv = malloc(sizeof(Atom));
    nv->type = AS_BINDING;
    nv->bindval = bind;
    registerObject(collector, nv);
    return nv;
}

Atom* makeFunctionValue(Function* function) {
    Atom* nv = malloc(sizeof(Atom));
    nv->type = AS_FUNCTION;
    nv->funcval = function;
    registerObject(collector, nv);
    return nv;
}

Function* makePrimitveFunction(Atom* (*func)(List*)) {
    Function* function = malloc(sizeof(Function));
    function->type = PRIMITIVE;
    function->freeVars = createList();
    function->func = func;
    function->env = NULL;
    return function;
}

Function* makeLambdaFunction(List* args, List* body, List* env) {
    Function* func = malloc(sizeof(Function));
    func->type = LAMBDA;
    func->code = makeListVal(body);
    func->freeVars = args;
    func->env = env;
    return func;
}

void printValue(Atom* value) {
    switch (value->type) {
        case AS_NUM: printf("%d", value->intval); break;
        case AS_SYMBOL: printf("%s", value->stringval->data); break;
        case AS_LIST: printList(value->listval); break;
        case AS_BOOL: printf("%s", value->boolval ? "true":"false"); break;
        case AS_BINDING: {
            printf("{ "); printValue(value->bindval->symbol); printf(", "); printValue(value->bindval->value); printf(" }");
        } break;
        case AS_FUNCTION: printf("(lambda)"); break;
        case AS_NIL: printf("()"); break;
        default: break;
    }
}

bool is_literal(Atom* value) {
    if (value == NULL)
        return false;
    switch (value->type) {
        case AS_NUM:
        case AS_BOOL: 
        case AS_ERROR: return true;
        default: break;
    }
    return false;
}

bool is_function(Atom* value) {
    return value == NULL ? false:value->type == AS_FUNCTION;
}

bool is_primitive(Function* func) {
    return func == NULL ? false:func->type == PRIMITIVE;
}

bool is_lambda(Function* func) {
    return func == NULL ? false:func->type == LAMBDA;
}

bool is_binding(Atom* value) {
    return value == NULL ? false:value->type == AS_BINDING;
}

bool is_symbol(Atom* value) {
    return value == NULL ? false:value->type == AS_SYMBOL;
}

bool is_list(Atom* value) {
    return value == NULL ? false:value->type == AS_LIST;
}

void freeValue(Atom* value) {
    if (value == NULL)
        return;
    switch (value->type) {
        case AS_BOOL:
        case AS_NUM: {
            free(value);
        } break;
        case AS_SYMBOL: {
            freeString(value->stringval);
            free(value);
        } break;
        case AS_BINDING: {
            freeBinding(value->bindval);
            free(value);
        } break;
        case AS_LIST: {
            freeList(value->listval);
            free(value);
        } break;
        case AS_FUNCTION:
        default: break;
    }
}

void freeBinding(Binding* binding) {
    if (binding == NULL)
        return;
    freeValue(binding->symbol);
    freeValue(binding->value);
    free(binding);
}

void freeString(String* str) {
    if (str == NULL)
        return;
    free(str->data);
    free(str);
}

void initGC() {
    collector = malloc(sizeof(GC));
    collector->objList = createList();
}

void registerObject(GC* gc, Atom* val) {
    val->mark = WHITE;
    gc->objList = appendList(gc->objList, val);
}

List* mark(List* env) {
    if (env == NULL)
        return env;
    listnode* x = env->head;
    while (x != NULL) {
        //printf("Mark: "); printValue(x->info); printf("\n");
        x->info->mark = GREY;
        if (x->info->type == AS_BINDING) {
            x->info->bindval->symbol->mark = GREY;
            x->info->bindval->value->mark = GREY;
            if (x->info->bindval->value->type == AS_LIST)
                mark(x->info->bindval->value->listval);
        } else if (x->info->type == AS_LIST) {
            mark(x->info->listval);
        }
        x = x->next;
    }
    return env;
}

List* sweep(List* env) {
    listnode* x = collector->objList->head;
    listnode* p = x; x = x->next;
    int frc = 0, nw = 0;
    while (x != NULL) {
        if (x->info->mark != GREY) {
            /*switch (x->info->type) {
                case AS_NUM:     printf("num:      "); break;
                case AS_BOOL:    printf("bool:     "); break;
                case AS_BINDING: printf("binding:  "); break;
                case AS_LIST:    printf("List:     "); break;
                case AS_SYMBOL:  printf("symbol:   "); break;
                case AS_FUNCTION:printf("function: "); break;
            }
            printValue(x->info);
            printf(" has become unreachable, collecting.\n");*/
            p->next = p->next->next;
            x->next = NULL;
            free(x);
            x = p->next;
            frc++;
        } else {
            p = x;
            x = x->next;
            p->info->mark = WHITE;
            nw++;
        }
    }
    int k = 0;
    x = collector->objList->head;
    while (x->next != NULL) { x = x->next; k++; }
    collector->objList->tail = x;
    collector->objList->count = k;
    //printf("%d Objects collected, %d colored WHITE\n", frc, nw);
    return env;
}

List* addBindingToEnvironment(List* env, Binding* binding) {
    env = appendList(env, makeBindingVal(binding));
    return env;
}

List* createPrimitive(List* env, String* symbol, Atom* (func)(List*)) {
    env = addBindingToEnvironment(env, makeBinding(makeStringVal(symbol), 
                                                   makeFunctionValue(makePrimitveFunction(func))));
    return env;
}
