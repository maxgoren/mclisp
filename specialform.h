#ifndef specialform_h
#define specialform_h
#include "atom.h"
#include "list.h"
#include "eval.h"

Atom* sfDefine(List*, List*);
Atom* sfIf(List*,List*);
Atom* sfLambda(List*,List*);
Atom* sfSet(List*,List*);
Atom* sfQuote(List*,List*);
Atom* sfDo(List*,List*);
Atom* sfCond(List*,List*);
Atom* createSpecialForm(String* name, int numargs, char flags[], Atom* (*func)(List*,List*));
void initSpecialForms();
void setFlags(char flags[], int a, int b, int c);
SpecialForm* findSpecialForm(String* name);
#endif