#ifndef mgclisp_h
#define mgclisp_h
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "parse.h"
#include "atom.h"
#include "list.h"
#include "eval.h"
#include "primitives.h"
#include "specialform.h"

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

#define MAJOR_VERSION 0
#define MINOR_VERSION 61

extern List* env;
List* init(List*);
void repl();
char* getfilename(char* buff);
void runScript(char* filename, List* env);
void showVersion();

#endif