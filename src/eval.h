#ifndef eval_h
#define eval_h
#include "list.h"
#include "atom.h"

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

Atom* applySpecialForm(SpecialForm* sf, List* args, List* env);
Atom* applyMathPrim(char op, List* args);
Atom* apply(Function* func, List* args, List* env);
List* evalArgs(List* list, List* env);
Atom* eval(Atom* val, List* env);
Atom* evalList(List* list, List* env);
Atom* envLookUp(List* env, Atom* symbol);
List* envInsert(List* env, Atom* symbol);
List* makeNewEnvironment(List* vars, List* vals);
extern int dep;
extern bool trace_eval;
void enter(char* str);
void say(char* str);
void leave();

#endif
