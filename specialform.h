#ifndef specialform_h
#define specialform_h
#include "atom.h"
#include "list.h"
#include "eval.h"

/*
    MIT License

    (C) 2025 Max Goren, MaxGCoding.com
    
    mgclisp - a scheme interpreter in c
    Just for kicks.
    
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
    and associated documentation files (the “Software”), to deal in the Software without restriction, 
    including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
    and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
    subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or 
    substantial portions of the Software.

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
    OTHER DEALINGS IN THE SOFTWARE.

*/

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