#include "specialform.h"
#include "list.h"
#include <stdio.h>


Atom* sfDefine(List* args, List* env) {
    Atom* label = first(args);
    Atom* value = first(rest(args)->listval);
    String* nstr = makeString(label->stringval->data, label->stringval->len);
    env = envInsert(env, makeBindingAtom(makeBinding(makeSymbolAtom(nstr), value)));
    return label;
}

Atom* sfIf(List* args, List* env) {
    Atom* cond = args->head->info;
    Atom* istru = args->head->next->info;
    Atom* isfalse = args->head->next->next->info;
    if (cond->boolval)
        return eval(istru, env);
    return eval(isfalse, env);
}

Atom* sfLambda(List* args, List* env) {
    Function* func = malloc(sizeof(Function));
    func->type = LAMBDA;
    func->code = first(rest(args)->listval);
    func->freeVars = first(args)->listval;
    func->env = env;
    return makeFunctionAtom(func);
}

Atom* specialLet(List* args, List* env) {
    List* vars = first(args)->listval;
    List* body = first(rest(args)->listval)->listval;
    List* vn = createList();
    List* vv = createList();
    for (listnode* it = vars->head; it != NULL; it = it->next) {
        if (it->info->type == AS_LIST) {
            List* asList = it->info->listval;
            Atom* sym = asList->head->info;
            Atom* val = asList->head->next->info;
            vn = appendList(vn, sym);
            vv = appendList(vv, val);
        }
    }
    Function* func = makeLambdaFunction(vn, body, env);
    List* expr = createList();
    expr = appendList(expr, makeFunctionAtom(func));
    expr = addMissing(expr, vv);
    return eval(makeListAtom(expr), env);
}

Atom* specialSet(List* args, List* env) {
    Atom* sym = first(args);
    Atom* next = first(rest(args)->listval);
    for (listnode* it = env->head; it != NULL; it = it->next) {
        if (compareValue(sym, it->info->bindval->symbol)) {
            it->info->bindval->value = next;
            return next;
        }
    }
    env = appendList(env, makeBindingAtom(makeBinding(sym, next)));
    return next;
}

Atom* specialCond(List* args, List* env) {
    for (listnode* it = args->head; it != NULL; it = it->next) {
        Atom* test = first(it->info->listval);
        Atom* act = first(rest(it->info->listval)->listval);
        if (test->type == AS_SYMBOL && compareString(makeString("else", 4),test->stringval))
            return eval(act, env); 
        if (eval(test, env)->boolval)
            return eval(act, env);
    }
    return makeListAtom(createList());
}

Atom* sfDo(List* args, List* env) {
    Atom* result = makeIntAtom(0);
    for (listnode* it = args->head; it != NULL; it = it->next) {
        result = eval(it->info, env);
    }
    return result;
}

Atom* sfQuote(List* args, List* env) {
    return first(args);
}

Atom* createSpecialForm(String* name, int numargs, char flags[], Atom* (*func)(List*,List*)) {
    SpecialForm* sf = malloc(sizeof(SpecialForm));
    sf->name = name;
    sf->numArgs = numargs;
    for (int i = 0; i < numargs; i++)
        sf->flags[i] = flags[i];
    sf->func = func;
    Atom* vv = malloc(sizeof(Atom));
    vv->sf = sf;
    vv->type = AS_SF;
    return vv;
}

List* specialForms;

void setFlags(char flags[], int a, int b, int c) {
    flags[0] = a;
    flags[1] = b;
    flags[2] = c;
}

void initSpecialForms() {
    char flags[3];
    specialForms = createList();
    setFlags(flags, NO_EVAL, NO_EVAL, NO_EVAL);
    specialForms = appendList(specialForms, createSpecialForm(makeString("let",3), 2, flags, &specialLet));
    specialForms = appendList(specialForms, createSpecialForm(makeString("lambda", 6), 2, flags, &sfLambda));
    specialForms = appendList(specialForms, createSpecialForm(makeString("&", 1), 2, flags, &sfLambda));
    specialForms = appendList(specialForms, createSpecialForm(makeString("'", 1), 0, flags, &sfQuote));
    specialForms = appendList(specialForms, createSpecialForm(makeString("quote", 1), 0, flags, &sfQuote));
    specialForms = appendList(specialForms, createSpecialForm(makeString("cond",4), 0, flags, &specialCond));
    specialForms = appendList(specialForms, createSpecialForm(makeString("do",2), 0, flags, &sfDo));
    setFlags(flags, NO_EVAL, EVAL, NO_EVAL);
    specialForms = appendList(specialForms, createSpecialForm(makeString("define", 6), 2, flags, &sfDefine));
    specialForms = appendList(specialForms, createSpecialForm(makeString("set!",3), 2, flags, &specialSet));
    setFlags(flags, EVAL, NO_EVAL, NO_EVAL);
    specialForms = appendList(specialForms, createSpecialForm(makeString("if", 2), 3, flags, &sfIf));
}

SpecialForm* findSpecialForm(String* name) {
    for (listnode* it = specialForms->head; it != NULL; it = it->next) {
        SpecialForm* sf = it->info->sf;
        if (compareString(name, sf->name)) {
            return sf;
        }
    }
    return NULL;
}