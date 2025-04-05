#ifndef primitives_h
#define primitives_h
#include "list.h"
#include "value.h"

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