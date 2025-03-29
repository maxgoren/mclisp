#include "eval.h"
#include "specialform.h"
#include <stdio.h>

int dep = 0;

void enter(char* str) {
    dep++;
    say(str);
}
void say(char* str) {
    for (int i = 0; i < dep; i++) printf("  ");
    printf("%s ", str);
}
void leave() {
    dep--;
}

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

int d = 0;

Value* eval(Value* value, List* env) {
    enter("eval("); printValue(value); printf(")\n");
    leave();
    switch (value->type) {
        case AS_NUM: return value;
        case AS_BOOL: return value;
        case AS_FUNCTION: return value;
        case AS_ERROR: return value;
        case AS_SYMBOL: return envLookUp(env, value);
        case AS_LIST: {
            return evalList(value->listval, env);
        }
        default: break;
    }
    return makeListVal(createList());
}

Value* evalList(List* list, List* env) {
    enter("evallist("); printList(list); printf(")\n");
    if (listEmpty(list))
        return list;
    if (first(list)->type == AS_SYMBOL && findSpecialForm(first(list)->stringval) != NULL) {
        leave();
        return applySpecialForm(findSpecialForm(first(list)->stringval), rest(list)->listval, env);
    }
    //Evalute Arguments
    List* evald = createList();
    for (listnode* it = list->head; it != NULL; it = it->next) {
        evald = appendList(evald, eval(it->info, env));
    }
    //Apply function
    if (first(evald)->type == AS_FUNCTION) {
        leave();
        return apply(first(evald)->funcval, rest(evald)->listval, env);
    }
    leave();
    return makeListVal(evald);
}

Value* applySpecialForm(SpecialForm* sf, List* args, List* env) {
    enter("apply special("); printf("%s ", sf->name->data); printList(args); printf(")\n");
    List* evald = createList();
    listnode* it = args->head; 
    int i = 0; 
    while (it != NULL) {
        Value* result = it->info;
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

Value* apply(Function* func, List* args, List* env) {
    enter("apply("); printList(args); printf(")\n");
    if (func->type == PRIMITIVE) {
        say("function is a primitive\n");
        leave();
        return func->func(args);
    }
    if (func->type == LAMBDA) {
        say("function is a lambda\n");
        List* nenv = makeNewEnvironment(func->freeVars, args);
        nenv = addMissing(nenv, func->env);
        nenv = addMissing(nenv, env);
        leave();
        return eval(func->code, nenv);
    }
    leave();
    return makeListVal(createList());
}

Value* applyMathPrim(char op, List* list) {
    char msg[255];
    sprintf(msg, "apply math prim (%c ", op);
    enter(msg);
    printList(list);
    printf("\n");
    int result = first(list)->intval;
    for (listnode* it = rest(list)->listval->head; it != NULL; it = it->next) {
        switch (op) {
            case '+': { result += it->info->intval; } break;
            case '-': { result -= it->info->intval; } break;
            case '*': { result *= it->info->intval; } break;
            case '/': { 
                if (it->info->intval == 0) {
                    leave();
                    return makeIntVal(0);
                }
                result /= it->info->intval; 
            } break;
            case '%': {
                if (it->info->intval == 0) {
                    leave();
                    return makeIntVal(0);
                }
                result = result % it->info->intval; 
            } break;
            case '<': { result = (it->info->intval < result) ? it->info->intval:result; } break;
            case '>': { result = (it->info->intval > result) ? it->info->intval:result; } break;
            default: break;
        }
    }
    leave();
    return makeIntVal(result);
}

