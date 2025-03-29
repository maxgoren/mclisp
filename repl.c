#include <stdio.h>
#include <ctype.h>
#include "string.h"
#include <stdlib.h>
#include "parse.h"
#include "value.h"
#include "list.h"
#include "eval.h"
#include "primitives.h"
#include "specialform.h"
/*
    (define fib (& (x) (do (define fib-iter (& (curr prev cnt max) (if (eq cnt max) (say curr) (fib-iter (+ prev curr) curr (+ cnt 1) max)))) (fib-iter 1 1 1 x))))
    (define fact (& (x) (do (define fact-iter (& (prod cnt max) (if (eq cnt max) (* prod cnt) (fact-iter (* prod cnt) (+ 1 cnt) max)))) (fact-iter 1 1 x))))
    
*/    
#define NUM_PREDEFS 5

char *funcs[NUM_PREDEFS] = {
    "(define empty? (& (x) (eq x () ) ) )",
    "(define count? (& (xs) (if (empty? xs) 0 (+ 1 (count (cdr xs) ) ) ) ) )",
    "(define map (& (f xs) (if (empty? xs) () (push (f (car xs)) (map f (cdr xs) ) ) ) ) )",
    "(define filter (& (f xs) (if (empty? xs) () (if (f (car xs)) (push (car xs) (filter f (cdr xs))) (filter f (cdr xs))) )",
    "(define nth (& (n xs) (if (eq n 0) (car xs) (nth (- n 1) (cdr xs)) ) ) )"
};

List* init(List* env) {
    printf("Initalizing mgclisp...\n");
    initGC();
    printf("GC Initialized...\n");
    initSpecialForms();
    printf("Loading Environment...\n");
    env = makePrim(env, makeString("id", 2), &primId);
    env = makePrim(env, makeString("+", 1), &primPlus);
    env = makePrim(env, makeString("-", 1), &primMinus);
    env = makePrim(env, makeString("*", 1), &primMul);
    env = makePrim(env, makeString("/", 1), &primDiv);
    env = makePrim(env, makeString("%", 1), &primMod);
    env = makePrim(env, makeString("car", 3), &primCar);
    env = makePrim(env, makeString("cdr", 3), &primCdr);
    env = makePrim(env, makeString("cons", 4), &primCons);
    env = makePrim(env, makeString("append", 6), &primAppend);
    env = makePrim(env, makeString("eq", 2), &primEqual);
    env = makePrim(env, makeString("lt", 2), &primLess);
    env = makePrim(env, makeString("gte", 3), &primGreaterEq);
    env = makePrim(env, makeString("lte", 3), &primLessEq);
    env = makePrim(env, makeString("gt", 2), &primGreater);
    env = makePrim(env, makeString("say", 3),&primPrint);
    env = makePrim(env, makeString("and", 3), &primAnd);
    env = makePrim(env, makeString("not", 3),&primNot);
    env = makePrim(env, makeString("or", 2), &primOr);
    env = makePrim(env, makeString("min", 3), &primMin);
    env = makePrim(env, makeString("max", 3), &primMax);
    env = makePrim(env, makeString("eq?", 3), &primSymbolicEquality);
    env = makePrim(env, makeString("list", 4), &primList);
    env = makePrim(env, makeString("join", 4), &primJoin);
    for (int i = 0; i < NUM_PREDEFS; i++)
        eval(makeListVal(stringToList(funcs[i])), env);
    printf("Enviornment Loaded Successfully.\n");
    return env;
}

void repl() {
    List* env = createList();
    env = init(env);
    int ecnt = 0;
    char buff[255];
    for (;;) {
        printf("misp(%d)> ", ecnt++);
        fgets(buff, sizeof(buff), stdin);
        if (strcmp(buff, "quit") == 0)
            return;
        List* asList = stringToList(buff);
        printList(asList);
        printf("\n => ");
        printValue(eval(makeListVal(asList), env));
        printf("\n");
        env = mark(env);
        env = sweep(env);
        memset(buff, '\0', sizeof(buff));
    }
}

int main() {
    repl();
}

