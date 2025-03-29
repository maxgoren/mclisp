#include <stdio.h>
#include <string.h>
#include "value.h"
#include "list.h"
#include "primitives.h"
#include "specialform.h"

GC* collector;

bool compareValue(Value* lhs, Value* rhs) {
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

Value* makeStringVal(String* str) {
    Value* nv = malloc(sizeof(Value));
    nv->type = AS_SYMBOL;
    nv->stringval = str;
    registerObject(collector, nv);
    return nv;
}

Value* makeError(String* str) {
    Value* nv = malloc(sizeof(Value));
    nv->type = AS_ERROR;
    nv->stringval = str;
    registerObject(collector, nv);
    return nv;
}

Value* makeError(char* str) {
    Value* nv = malloc(sizeof(Value));
    nv->type = AS_ERROR;
    nv->stringval = makeString(str, strlen(str));
    registerObject(collector, nv);
    return nv;
}

Value* makeIntVal(int value) {
    Value* nv = malloc(sizeof(Value));
    nv->intval = value;
    nv->type = AS_NUM;
    registerObject(collector, nv);
    return nv;
}

Value* makeBoolVal(bool value) {
    Value* nv = malloc(sizeof(Value));
    nv->boolval = value;
    nv->type = AS_BOOL;
    registerObject(collector, nv);
    return nv;
}

Value* makeListVal(List* list) {
    Value* nv = malloc(sizeof(Value));
    nv->type = AS_LIST;
    nv->listval = list;
    registerObject(collector, nv);
    return nv;
}

Binding* makeBinding(Value* symbol, Value* value) {
    Binding* binding = malloc(sizeof(Binding));
    binding->symbol = symbol;
    binding->value = value;
    return binding;
}

Value* makeBindingVal(Binding* bind) {
    Value* nv = malloc(sizeof(Value));
    nv->type = AS_BINDING;
    nv->bindval = bind;
    registerObject(collector, nv);
    return nv;
}

Value* makeFunctionValue(Function* function) {
    Value* nv = malloc(sizeof(Value));
    nv->type = AS_FUNCTION;
    nv->funcval = function;
    registerObject(collector, nv);
    return nv;
}

Function* makePrimitveFunction(Value* (*func)(List*)) {
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

void printValue(Value* value) {
    switch (value->type) {
        case AS_NUM: printf("%d", value->intval); break;
        case AS_SYMBOL: printf("%s", value->stringval->data); break;
        case AS_LIST: printList(value->listval); break;
        case AS_BOOL: printf("%s", value->boolval ? "true":"false"); break;
        case AS_BINDING: {
            printf("{ "); printValue(value->bindval->symbol); printf(", "); printValue(value->bindval->value); printf(" }");
        } break;
        case AS_FUNCTION: printf("(lambda)"); break;
        default: break;
    }
}

void freeValue(Value* value) {
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

void registerObject(GC* gc, Value* val) {
    val->mark = WHITE;
    gc->objList = appendList(gc->objList, val);
}

List* mark(List* env) {
    if (env == NULL)
        return env;
    listnode* x;
    x = env->head;
    listnode* tx = x;
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
    env->head = tx;
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

List* bindSymbolToValue(List* env, Binding* binding) {
    env = appendList(env, makeBindingVal(binding));
    return env;
}

List* makePrim(List* env, String* symbol, Value* (func)(List*)) {
    Binding* binding = makeBinding(makeStringVal(symbol), makeFunctionValue(makePrimitveFunction(func)));
    env = bindSymbolToValue(env, binding);
    return env;
}
