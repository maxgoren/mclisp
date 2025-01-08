#ifndef specialform_h
#define specialform_h
#include "value.h"
#include "list.h"
#include "eval.h"

Value* specialDefine(List*, List*);
Value* specialIf(List*,List*);
Value* specialLambda(List*,List*);
Value* speicalSet(List*,List*);
Value* specialQuote(List*,List*);
Value* specialDo(List*,List*);
Value* specialCons(List*,List*);
Value* makeSpecial(String* name, int numargs, char flags[], Value* (*func)(List*,List*));
void initSpecials();
SpecialForm* checkSpecials(String* name);
Value* applySpecial(SpecialForm* sf, List* args, List* env);
#endif