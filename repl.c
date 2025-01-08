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

//(define fib (& (x) (do (define fib-iter (& (curr prev cnt max) (if (eq cnt max) (say curr) (fib-iter (+ prev curr) curr (+ cnt 1) max)))) (fib-iter 1 1 1 x))))

void init(List* env) {
    initGC();
    initSpecials();
    env = addPrimitive(env, makeString("id", 2), &primId);
    env = addPrimitive(env, makeString("+", 1), &primPlus);
    env = addPrimitive(env, makeString("-", 1), &primMinus);
    env = addPrimitive(env, makeString("*", 1), &primMul);
    env = addPrimitive(env, makeString("/", 1), &primDiv);
    env = addPrimitive(env, makeString("car", 3), &primCar);
    env = addPrimitive(env, makeString("cdr", 3), &primCdr);
    env = addPrimitive(env, makeString("cons", 4), &primCons);
    env = addPrimitive(env, makeString("eq", 2), &primEqual);
    env = addPrimitive(env, makeString("lt", 2), &primLess);
    env = addPrimitive(env, makeString("gte", 3), &primGreaterEq);
    env = addPrimitive(env, makeString("lte", 3), &primLessEq);
    env = addPrimitive(env, makeString("gt", 2), &primGreater);
    env = addPrimitive(env, makeString("say", 3),&primPrint);
    env = addPrimitive(env, makeString("and", 3), &primAnd);
    env = addPrimitive(env, makeString("not", 3),&primNot);
    env = addPrimitive(env, makeString("or", 2), &primOr);
}

char *funcs[4] = {
    "(define empty? (& (x) (eq x () ) ) )",
    "(define count? (& (lst) (if (empty? lst) 0 (+ 1 (count (cdr lst) ) ) ) ) )",
    "(define map (& (f lst) (if (empty? lst) () (push (f (car lst)) (map f (cdr lst) ) ) ) ) )",
    "(define filter (& (f lst) (if (empty? lst) () (if (f (car lst)) (push (car lst) (filter f (cdr lst))) (filter f (cdr lst))) )"
};

void repl() {
    List* env = createList();
    init(env);
    for (int i = 0; i < 4; i++)
        eval(makeListVal(stringToList(funcs[i])), env);
    env = mark(env);
    env = sweep(env);
    int ecnt = 0;
    char buff[255];
    for (;;) {
        printf("misp(%d)> ", ecnt++);
        fgets(buff, sizeof(buff), stdin);
        if (strcmp(buff, "quit") == 0)
            return;
        for (int m = 0;buff[m];m++) if (buff[m] == '\n') { buff[m] = '\0'; break; }
        List* asList = stringToList(buff);
        printList(asList);
        printf("\n => ");
        printValue(eval(makeListVal(asList), env));
        printf("\n");
        env = mark(env);
        env = sweep(env);
    }
}

int main() {
    repl();
}

