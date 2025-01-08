#include "specialform.h"
#include "list.h"
#include <stdio.h>



Value* specialDefine(List* args, List* env) {
    Value* label = first(args);
    Value* value = first(rest(args)->listval);
    String* nstr = makeString(label->stringval->data, label->stringval->len);
    env = envInsert(env, makeBindingVal(makeBinding(makeStringVal(nstr), value)));
    return label;
}

Value* specialIf(List* args, List* env) {
    Value* cond = args->head->info;
    Value* istru = args->head->next->info;
    Value* isfalse = args->head->next->next->info;
    if (cond->boolval)
        return eval(istru, env);
    return eval(isfalse, env);
}

Value* specialLambda(List* args, List* env) {
    Function* func = malloc(sizeof(Function));
    func->type = LAMBDA;
    func->code = first(rest(args)->listval);
    func->freeVars = first(args)->listval;
    func->env = env;
    return makeFunctionValue(func);
}

Value* specialLet(List* args, List* env) {
    List* vars = first(args)->listval;
    List* body = first(rest(args)->listval)->listval;
    List* vn = createList();
    List* vv = createList();
    for (listnode* it = vars->head; it != NULL; it = it->next) {
        if (it->info->type == AS_LIST) {
            List* asList = it->info->listval;
            Value* sym = asList->head->info;
            Value* val = asList->head->next->info;
            printValue(sym);
            printValue(val);
            printf("\n");
            vn = appendList(vn, sym);
            vv = appendList(vv, val);
        }
    }
    Function* func = makeLambdaFunction(vn, body, env);
    List* expr = createList();
    expr = appendList(expr, makeFunctionValue(func));
    expr = addMissing(expr, vv);
    return eval(makeListVal(expr), env);
}

Value* specialSet(List* args, List* env) {
    Value* sym = first(args);
    Value* next = first(rest(args)->listval);
    for (listnode* it = env->head; it != NULL; it = it->next) {
        if (compareValue(sym, it->info->bindval->symbol)) {
            it->info->bindval->value = next;
            return next;
        }
    }
    env = appendList(env, makeBindingVal(makeBinding(sym, next)));
    return next;
}

Value* specialCond(List* args, List* env) {
    for (listnode* it = args->head; it != NULL; it = it->next) {
        Value* test = first(it->info->listval);
        Value* act = first(rest(it->info->listval)->listval);
        if (eval(test, env)->boolval)
            return eval(act, env);
    }
    return makeListVal(createList());
}

Value* specialDo(List* args, List* env) {
    Value* result = makeIntVal(0);
    for (listnode* it = args->head; it != NULL; it = it->next) {
        result = eval(it->info, env);
    }
    return result;
}

Value* specialQuote(List* args, List* env) {
    return first(args);
}

SpecialForm* makeSpecial(String* name, int numargs, char flags[], Value* (*func)(List*,List*)) {
    SpecialForm* sf = malloc(sizeof(SpecialForm));
    sf->name = name;
    sf->numArgs = numargs;
    for (int i = 0; i < numargs; i++)
        sf->flags[i] = flags[i];
    sf->func = func;
    sf->next = NULL;
    return sf;
}

SpecialForm* specialForms;


void initSpecials() {
    char flags[3];
    flags[0] = NO_EVAL;
    flags[1] = EVAL;
    flags[2] = NO_EVAL;
    specialForms = makeSpecial(makeString("define", 6), 2, flags, &specialDefine);
    SpecialForm* t = specialForms;
    flags[1] = NO_EVAL;
    t->next = makeSpecial(makeString("lambda", 6), 2, flags, &specialLambda);
    t = t->next;
    t->next = makeSpecial(makeString("&", 1), 2, flags, &specialLambda);
    t = t->next;
    t->next = makeSpecial(makeString("'", 1), 0, flags, &specialQuote);
    t = t->next;
    flags[0] = EVAL; 
    t->next = makeSpecial(makeString("if", 2), 3, flags, &specialIf);
    t = t->next;
    flags[0] = NO_EVAL;
    t->next = makeSpecial(makeString("let",3), 2, flags, &specialLet);
    t = t->next;
    flags[1] = EVAL;
    t->next = makeSpecial(makeString("set",3), 2, flags, &specialSet);
    t = t->next;
    t->next = makeSpecial(makeString("cond",4), 0, flags, &specialCond);
    t = t->next;
    t->next = makeSpecial(makeString("do",2), 0, flags, &specialDo);
    
}

SpecialForm* checkSpecials(String* name) {
    for (SpecialForm* it = specialForms; it != NULL; it = it->next) {
        if (compareString(name, it->name)) {
            return it;
        }
    }
    return NULL;
}

Value* applySpecial(SpecialForm* sf, List* args, List* env) {
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
    return sf->func(evald, env);
}