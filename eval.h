#ifndef eval_h
#define eval_h
#include "list.h"
#include "value.h"

Value* applySpecialForm(SpecialForm* sf, List* args, List* env);
Value* applyMathPrim(char op, List* args);
Value* apply(Function* func, List* args, List* env);
Value* eval(Value* val, List* env);
Value* evalList(List* list, List* env);
Value* envLookUp(List* env, Value* symbol);
List* envInsert(List* env, Value* symbol);
List* makeNewEnvironment(List* vars, List* vals);

extern int dep;
void enter(char* str);
void say(char* str);
void leave();

#endif
