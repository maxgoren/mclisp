#include "eval.h"
#include "specialform.h"
#include <stdio.h>

List* makeNewEnvironment(List* vars, List* vals) {
    List* nenv = createList();
    listnode* var = vars->head;
    listnode* val = vals->head;
    while (var != NULL && val != NULL) {
        nenv = appendList(nenv, makeBindingVal(makeBinding(var->info, val->info)));
        var = var->next;
        val = val->next;
    }
    return nenv;
}

List* envInsert(List* env, Value* symbol) {
    for (listnode* it = env->head; it != NULL; it = it->next) {
        if (compareValue(symbol->bindval->symbol, it->info->bindval->symbol)) {
            printf("Error: A value is already associated to that symbol\n");
            return env;
        }
    }
    return appendList(env, symbol);
}

Value* envLookUp(List* env, Value* symbol) {
    for (listnode* it = env->head; it != NULL; it = it->next) {
        if (compareValue(symbol, it->info->bindval->symbol)) {
            return it->info->bindval->value;
        }
    }
    return makeListVal(createList());
}

Value* eval(Value* value, List* env) {
    switch (value->type) {
        case AS_NUM: return value;
        case AS_BOOL: return value;
        case AS_FUNCTION: return value;
        case AS_SYMBOL: return envLookUp(env, value);
        case AS_LIST: {
            return evalList(value->listval, env);
        }
        default: break;
    }
    return makeListVal(createList());
}

Value* evalList(List* list, List* env) {
    //check special forms
    if (first(list)->type == AS_SYMBOL && checkSpecials(first(list)->stringval) != NULL) {
        return applySpecial(checkSpecials(first(list)->stringval), rest(list)->listval, env);
    }
    //Evalute Arguments
    List* evald = createList();
    for (listnode* it = list->head; it != NULL; it = it->next) {
        evald = appendList(evald, eval(it->info, env));
    }
    //Apply function
    if (first(evald)->type == AS_FUNCTION) {
        return apply(first(evald)->funcval, rest(evald)->listval, env);
    }
    //return result as listval
    return makeListVal(evald);
}

Value* apply(Function* func, List* args, List* env) {
    if (func->type == PRIMITIVE) {
        //printf("function is a primitive\n");
        return func->func(args);
    }
    if (func->type == LAMBDA) {
        //printf("function is a lambda\n");
        List* nenv = makeNewEnvironment(func->freeVars, args);
        nenv = addMissing(nenv, env);
        nenv = addMissing(nenv, func->env);
        return eval(func->code, nenv);
    }
    return makeListVal(createList());
}

Value* applyMathPrim(char op, List* list) {
    int result = first(list)->intval;
    for (listnode* it = rest(list)->listval->head; it != NULL; it = it->next) {
        switch (op) {
            case '+': { result += it->info->intval; } break;
            case '-': { result -= it->info->intval; } break;
            case '*': { result *= it->info->intval; } break;
            case '/': { 
                if (it->info->intval == 0)
                    return makeIntVal(0);
                result /= it->info->intval; 
            } break;
            default: break;
        }
    }
    return makeIntVal(result);
}

