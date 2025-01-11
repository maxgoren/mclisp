#ifndef primitives_h
#define primitives_h
#include "list.h"
#include "value.h"

//primitive algebra
Value* primPlus(List* list);
Value* primMinus(List* list);
Value* primMul(List* list);
Value* primDiv(List* list);
Value* primMod(List* list);

//list ops
Value* primCar(List* list);
Value* primCdr(List* list);
Value* primCons(List* list);
Value* primAppend(List* list);
Value* primList(List* list);
Value* primJoin(List* list);

//primitive comparison
Value* primEqual(List* list);
Value* primLess(List* list);
Value* primGreater(List* list);
Value* primLessEq(List* list);
Value* primGreaterEq(List* list);

//
Value* primSymbolicEquality(List* list);
//I/O
Value* primPrint(List* list);

//Reflection
Value* primId(List* list);

//logic modifiers
Value* primOr(List* list);
Value* primAnd(List* list);
Value* primNot(List* list);

//utils
Value* primMin(List* list);
Value* primMax(List* list);

#endif