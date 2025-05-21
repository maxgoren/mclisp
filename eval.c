#include "eval.h"
#include "specialform.h"
#include <stdio.h>

int dep = 0;
Atom* NIL;
bool trace_eval = false;
void enter(char* str) {
    dep++;
    if (trace_eval) {
        say(str);
    }
}
void say(char* str) {
    if (trace_eval) {
        for (int i = 0; i < dep; i++) printf("  ");
        printf("%s ", str);
    }
}
void leave() {
    dep--;
}

Atom* error(char *str) {
    printf("!! Error: %s\n", str);
    return makeNil();
}

List* makeNewEnvironment(List* vars, List* vals) {
    List* nenv = createList();
    listnode* var = vars->head;
    listnode* val = vals->head;
    while (var != NULL && val != NULL) {
        nenv = appendList(nenv, makeBindingAtom(makeBinding(var->info, val->info)));
        var = var->next;
        val = val->next;
    }
    return nenv;
}

List* envInsert(List* env, Atom* symbol) {
    for (listnode* it = env->head; it != NULL; it = it->next) {
        if (compareValue(symbol->bindval->symbol, it->info->bindval->symbol)) {
            printf("Error: A value is already associated to that symbol\n");
            return env;
        }
    }
    return appendList(env, symbol);
}

Atom* envLookUp(List* env, Atom* symbol) {
    for (listnode* it = env->head; it != NULL; it = it->next) {
        if (compareValue(symbol, it->info->bindval->symbol)) {
            return it->info->bindval->value;
        }
    }
    return NIL;
}

int d = 0;

Atom* eval(Atom* value, List* env) {
    if (trace_eval) {
        enter("eval("); printValue(value); printf(")\n");
    }
    if (is_literal(value) || is_binding(value) || is_function(value)) { leave(); return value; }
    if (is_symbol(value)) { leave(); return envLookUp(env, value); }
    Atom* retval = makeNil();
    if (is_list(value)) {
        leave();
        return evalList(value->listval, env);
    }
    return makeNil();
}

Atom* evalList(List* list, List* env) {
    if (listEmpty(list))
        return makeListAtom(list);
    if (is_symbol(first(list))) {
        SpecialForm* sf = findSpecialForm(first(list)->stringval);
        if (sf != NULL) {
            return applySpecialForm(sf, rest(list)->listval, env);
        }
    }
    //Evalute Arguments
    List* evald = createList();
    for (listnode* it = list->head; it != NULL; it = it->next) {
        evald = appendList(evald, eval(it->info, env));
    }
    //Apply function
    if (is_function(first(evald))) {
        return apply(first(evald)->funcval, rest(evald)->listval, env);
    }
    return makeListAtom(evald);
}

Atom* applySpecialForm(SpecialForm* sf, List* args, List* env) {
    if (trace_eval) {
        enter("apply special("); printf("%s ", sf->name->data); printList(args); printf(")\n");
    }
    List* evald = createList();
    listnode* it = args->head; 
    int i = 0; 
    while (it != NULL) {
        Atom* result = it->info;
        if (i < sf->numArgs && sf->flags[i] == EVAL) {
            result = eval(result, env);
        }
        evald = appendList(evald, result);
        it = it->next;
        i++;
    }
    leave();
    return sf->func(evald, env);
}


/*
    Step 1) make new env with functions arguments associated to there values
    Step 2) merge with functions defining environment (lexical scoping)
    Step 3) merge with functions calling environment
*/
List* prepareEnvironment(Function* func, List* args, List* env) {
    List* nenv = makeNewEnvironment(func->freeVars, args);
    nenv = addMissing(nenv, func->env);
    nenv = addMissing(nenv, env);
    return nenv;
}

Atom* apply(Function* func, List* args, List* env) {
    if (trace_eval) {
        enter("apply("); printList(args); printf(")\n");
    }
    if (is_primitive(func)) {
        say("function is a primitive\n");
        leave();
        return func->func(args);
    }
    if (is_lambda(func)) {
        say("function is a lambda\n");
        List* nenv = prepareEnvironment(func, args, env);
        leave();
        return eval(func->code, nenv);
    }
    leave();
    return NIL;
}

Atom* applyMathPrim(char op, List* list) {
    char msg[255];
    if (trace_eval) {
        sprintf(msg, "apply math prim (%c ", op);
        enter(msg); printList(list); printf("\n");
    }
    int result = first(list)->intval;
    if (list->count == 1 && op == '-')
        return makeIntAtom(-result);
    else if (list->count == 1 && op == '+')
        return makeIntAtom(+result);
    for (listnode* it = rest(list)->listval->head; it != NULL; it = it->next) {
        switch (op) {
            case '+': { result += it->info->intval; } break;
            case '-': { result -= it->info->intval; } break;
            case '*': { result *= it->info->intval; } break;
            case '/': { 
                if (it->info->intval == 0) {
                    leave();
                    return error("!! ERROR: Divide by zero? Really!?");
                }
                result /= it->info->intval; 
            } break;
            case '%': {
                if (it->info->intval == 0) {
                    leave();
                    return error("!! ERROR: Divide by zero? Really!?");
                }
                result = result % it->info->intval; 
            } break;
            case '<': { result = (it->info->intval < result) ? it->info->intval:result; } break;
            case '>': { result = (it->info->intval > result) ? it->info->intval:result; } break;
            default: break;
        }
    }
    leave();
    return makeIntAtom(result);
}

