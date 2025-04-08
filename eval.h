#ifndef eval_h
#define eval_h
#include "list.h"
#include "atom.h"

Atom* applySpecialForm(SpecialForm* sf, List* args, List* env);
Atom* applyMathPrim(char op, List* args);
Atom* apply(Function* func, List* args, List* env);
Atom* eval(Atom* val, List* env);
Atom* evalList(List* list, List* env);
Atom* envLookUp(List* env, Atom* symbol);
List* envInsert(List* env, Atom* symbol);
List* makeNewEnvironment(List* vars, List* vals);
extern int dep;
void enter(char* str);
void say(char* str);
void leave();

#endif
