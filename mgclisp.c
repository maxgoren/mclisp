#include "mgclisp.h"

/*
    (define fib (& (x) (do (define fib-iter (& (curr prev cnt max) (if (eq cnt max) (say curr) (fib-iter (+ prev curr) curr (+ cnt 1) max)))) (fib-iter 1 1 1 x))))
    (define fact (& (x) (do (define fact-iter (& (prod cnt max) (if (eq cnt max) (* prod cnt) (fact-iter (* prod cnt) (+ 1 cnt) max)))) (fact-iter 1 1 x))))
    (define MUL (& (m n) (if (eq n 0) 0 (+ m (MUL m (- n 1))))))

*/  
 

List* init(List* env) {
    printf("Initalizing mgclisp...\n");
    initGC();
    NIL = makeListAtom(createList());
    printf("GC Initialized...\n");
    initSpecialForms();
    printf("Loading Primitives...\n");
    env = createPrimitive(env, makeString("id", 2), &primId);
    env = createPrimitive(env, makeString("+", 1), &primPlus);
    env = createPrimitive(env, makeString("-", 1), &primMinus);
    env = createPrimitive(env, makeString("*", 1), &primMul);
    env = createPrimitive(env, makeString("/", 1), &primDiv);
    env = createPrimitive(env, makeString("%", 1), &primMod);
    env = createPrimitive(env, makeString("car", 3), &primCar);
    env = createPrimitive(env, makeString("cdr", 3), &primCdr);
    env = createPrimitive(env, makeString("cons", 4), &primCons);
    env = createPrimitive(env, makeString("append", 6), &primAppend);
    env = createPrimitive(env, makeString("eq", 2), &primEqual);
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
    runScript("stdlib.scm", env);
    printf("MGCLisp Loaded Successfully.\n");
    return env;
}

void repl() {
    List* env = createList();
    env = init(env);
    int ecnt = 0;
    char buff[1024];
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
            printf(" => ");
            printValue(eval(makeListAtom(asList), env));
            printf("\n");
            env = runGC(env);
        }
        memset(buff, '\0', sizeof(buff));
    }
}

char* extractExpr(FILE* fd) {
    char* expr = malloc(sizeof(char) * 1024);
    int i = 0;
    int lp = 0, rp = 0;
    char ch;
    while ((ch = fgetc(fd)) != EOF) {
        if (ch == '(') {
            lp++;
            expr[i++] = ch;
            break;
        }
    }
    while ((ch = fgetc(fd)) != EOF) {
        if (ch == '(') lp++;
        else if (ch == ')') rp++;
        expr[i++] = ch;
        if (lp == rp)
            break;
    }
    expr[i++] = '\0';
    return expr;
}

void runScript(char* filename, List* env) {
    FILE* fd = fopen(filename, "r");
    if (fd == NULL) {
        printf("Error opening file %s, bailing out!", filename);
        return;
    } else {
        printf("Loading: %s\n", filename);
    }
    char ch;
    char *str;
    Atom* result;
    while ((ch = fgetc(fd)) != EOF) {
            str = extractExpr(fd);
            List* asList = stringToList(str);
            result = eval(makeListAtom(asList), env);
            printf(" => ");
            printValue(result);
            printf("\n");
    }
}

/*
(define (adjoin-set x set) 
        (cond ((empty? set) (make-tree x '() '()))
              ((eq x (entry set)) set)
              ((lt x (entry set)) (make-tree (entry set) 
                                              (adjoin-set x (left-branch set))  
                                              (right-branch set)))
               ((gt x (entry set)) (make-tree (entry set) 
                                              (left-branch set)
                                              (adjoin-set x (right-branch set))))))
(define (set-member? x set)
        (cond ((empty? set) set)
        ((lt x (entry set)) (set-member? x (left-branch set))
        ((gt x (entry set)) (set-member? x (right-branch set))
        (else (entry set))
        ))
*/

