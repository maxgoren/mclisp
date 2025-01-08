#ifndef primitives_h
#define primitives_h
#include "list.h"
#include "value.h"

Value* primPlus(List* list);
Value* primMinus(List* list);
Value* primMul(List* list);
Value* primDiv(List* list);
Value* primCar(List* list);
Value* primCdr(List* list);
Value* primCons(List* list);
Value* primEqual(List* list);
Value* primLess(List* list);
Value* primGreater(List* list);
Value* primLessEq(List* list);
Value* primGreaterEq(List* list);
Value* primPrint(List* list);
Value* primOr(List* list);
Value* primAnd(List* list);
Value* primNot(List* list);
Value* primId(List* list);

#endif