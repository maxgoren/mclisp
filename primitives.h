#ifndef primitives_h
#define primitives_h
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

//primitive algebra
Atom* primPlus(List* list);
Atom* primMinus(List* list);
Atom* primMul(List* list);
Atom* primDiv(List* list);
Atom* primMod(List* list);

//list ops
Atom* primCar(List* list);
Atom* primCdr(List* list);
Atom* primCons(List* list);
Atom* primAppend(List* list);
Atom* primList(List* list);
Atom* primJoin(List* list);

//primitive comparison
Atom* primEqual(List* list);
Atom* primLess(List* list);
Atom* primGreater(List* list);
Atom* primLessEq(List* list);
Atom* primGreaterEq(List* list);

//
Atom* primSymbolicEquality(List* list);
//I/O
Atom* primPrint(List* list);

//Reflection
Atom* primId(List* list);

//logic modifiers
Atom* primOr(List* list);
Atom* primAnd(List* list);
Atom* primNot(List* list);

//utils
Atom* primMin(List* list);
Atom* primMax(List* list);
Atom* primApply(List* list);

#endif