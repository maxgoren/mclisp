#ifndef specialform_h
#define specialform_h
#include "value.h"
#include "list.h"
#include "eval.h"

Value* sfDefine(List*, List*);
Value* sfIf(List*,List*);
Value* sfLambda(List*,List*);
Value* sfSet(List*,List*);
Value* sfQuote(List*,List*);
Value* sfDo(List*,List*);
Value* sfCons(List*,List*);
Value* createSpecialForm(String* name, int numargs, char flags[], Value* (*func)(List*,List*));
void initSpecialForms();
SpecialForm* findSpecialForm(String* name);
Value* applySpecialForm(SpecialForm* sf, List* args, List* env);
#endif