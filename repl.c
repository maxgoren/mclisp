#include <stdio.h>
#include <ctype.h>
#include "string.h"
#include <stdlib.h>
#include "parse.h"
#include "atom.h"
#include "list.h"
#include "eval.h"
#include "primitives.h"
#include "specialform.h"
/*
    (define fib (& (x) (do (define fib-iter (& (curr prev cnt max) (if (eq cnt max) (say curr) (fib-iter (+ prev curr) curr (+ cnt 1) max)))) (fib-iter 1 1 1 x))))
    (define fact (& (x) (do (define fact-iter (& (prod cnt max) (if (eq cnt max) (* prod cnt) (fact-iter (* prod cnt) (+ 1 cnt) max)))) (fact-iter 1 1 x))))
    
*/    
#define NUM_PREDEFS 6

char *funcs[NUM_PREDEFS] = {
    "(define empty? (& (x) (eq x () ) ) )",
    "(define count? (& (xs) (if (empty? xs) 0 (+ 1 (count? (cdr xs) ) ) ) ) )",
    "(define map (& (f xs) (if (empty? xs) () (cons (f (car xs)) (map f (cdr xs) ) ) ) ) )",
    "(define filter (& (f xs) (if (empty? xs) () (if (f (car xs)) (cons (car xs) (filter f (cdr xs))) (filter f (cdr xs) ) ) ) )",
    "(define nth (& (n xs) (if (eq n 0) (car xs) (nth (- n 1) (cdr xs)) ) ) )",
    "(define even? (& (n) (eq (mod n 2) 0) ) )"
};

List* init(List* env) {
    printf("Initalizing mgclisp...\n");
    initGC();
    NIL = makeListAtom(createList());
    printf("GC Initialized...\n");
    initSpecialForms();
    printf("Loading Primitives...\n");
    env = createPrimitive(env, makeString("id", 2), &primId);
    env = createPrimitive(env, makeString("+", 1), &primPlus);
    env = createPrimitive(env, makeString("add", 3), &primPlus);
    env = createPrimitive(env, makeString("-", 1), &primMinus);
    env = createPrimitive(env, makeString("sub", 3), &primMinus);
    env = createPrimitive(env, makeString("*", 1), &primMul);
    env = createPrimitive(env, makeString("mul", 3), &primMul);
    env = createPrimitive(env, makeString("/", 1), &primDiv);
    env = createPrimitive(env, makeString("div", 3), &primDiv);
    env = createPrimitive(env, makeString("%", 1), &primMod);
    env = createPrimitive(env, makeString("mod", 3), &primMod);
    env = createPrimitive(env, makeString("car", 3), &primCar);
    env = createPrimitive(env, makeString("cdr", 3), &primCdr);
    env = createPrimitive(env, makeString("cons", 4), &primCons);
    env = createPrimitive(env, makeString("append", 6), &primAppend);
    env = createPrimitive(env, makeString("eq", 2), &primEqual);
    env = createPrimitive(env, makeString("eq?", 3), &primEqual);
    env = createPrimitive(env, makeString("=", 1), &primEqual);
    env = createPrimitive(env, makeString("lt", 2), &primLess);
    env = createPrimitive(env, makeString("<", 1), &primLess);
    env = createPrimitive(env, makeString("gte", 3), &primGreaterEq);
    env = createPrimitive(env, makeString("lte", 3), &primLessEq);
    env = createPrimitive(env, makeString("gt", 2), &primGreater);
    env = createPrimitive(env, makeString(">", 1), &primGreater);
    env = createPrimitive(env, makeString("say", 3),&primPrint);
    env = createPrimitive(env, makeString("and", 3), &primAnd);
    env = createPrimitive(env, makeString("not", 3),&primNot);
    env = createPrimitive(env, makeString("or", 2), &primOr);
    env = createPrimitive(env, makeString("min", 3), &primMin);
    env = createPrimitive(env, makeString("max", 3), &primMax);
    env = createPrimitive(env, makeString("eq?", 3), &primSymbolicEquality);
    env = createPrimitive(env, makeString("list", 4), &primList);
    env = createPrimitive(env, makeString("join", 4), &primJoin);
    env = createPrimitive(env, makeString("apply", 5), &primApply);
    printf("Initializing Standard Library..");
    for (int i = 0; i < NUM_PREDEFS; i++) 
        eval(makeListAtom(stringToList(funcs[i])), env);
    printf("MGCLisp Loaded Successfully.\n");
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
        if (buff[0] == '.') {
            switch (buff[1]) {
                case 'q': return;
                case 't': trace_eval = !trace_eval; break;
                default: break;
            }
        } else {
            List* asList = stringToList(buff);
            printList(asList);
            printf("\n => ");
            printValue(eval(makeListAtom(asList), env));
            printf("\n");
            env = mark(env);
            env = sweep(env);
        }
        memset(buff, '\0', sizeof(buff));
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        repl();
    } else {
        trace_eval = true;
        repl();
    }
    return 0;
}

