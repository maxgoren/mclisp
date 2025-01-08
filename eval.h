#ifndef eval_h
#define eval_h
#include "list.h"
#include "value.h"

Value* applyMathPrim(char op, List* args);
Value* apply(Function* func, List* args, List* env);
Value* eval(Value* val, List* env);
Value* evalList(List* list, List* env);
Value* envLookUp(List* env, Value* symbol);
List* envInsert(List* env, Value* symbol);
List* makeNewEnvironment(List* vars, List* vals);

#endif
