#include <stdio.h>
#include "primitives.h"
#include "string.h"
#include "value.h"
#include "list.h"
#include "eval.h"

Value* primPlus(List* list) {
    return applyMathPrim('+', list);
}

Value* primMinus(List* list) {
    return applyMathPrim('-', list);
}

Value* primMul(List* list) {
    return applyMathPrim('*', list);
}

Value* primDiv(List* list) {
    return applyMathPrim('/', list);
}

Value* primCar(List* list) {
    return first(list->head->info->listval);
}

Value* primCdr(List* list) {
    return rest(list->head->info->listval);
}

Value* primEqual(List* list) {
    Value* lhs = first(list);
    Value* rhs = first(rest(list)->listval);
    return makeBoolVal(compareValue(lhs, rhs));
}

Value* primLess(List* list) {
    Value* lhs = first(list);
    Value* rhs = first(rest(list)->listval);
    if (lhs->type == AS_NUM && rhs->type == AS_NUM)
        return makeBoolVal(lhs->intval < rhs->intval);
    return makeListVal(createList());
}

Value* primGreater(List* list) {
    Value* lhs = first(list);
    Value* rhs = first(rest(list)->listval);
    if (lhs->type == AS_NUM && rhs->type == AS_NUM)
        return makeBoolVal(lhs->intval > rhs->intval);
    return makeListVal(createList());
}

Value* primLessEq(List* list) {
    Value* lhs = first(list);
    Value* rhs = first(rest(list)->listval);
    if (lhs->type == AS_NUM && rhs->type == AS_NUM)
        return makeBoolVal(lhs->intval <= rhs->intval);
    return makeListVal(createList());
}

Value* primGreaterEq(List* list) {
    Value* lhs = first(list);
    Value* rhs = first(rest(list)->listval);
    if (lhs->type == AS_NUM && rhs->type == AS_NUM)
        return makeBoolVal(lhs->intval >= rhs->intval);
    return makeListVal(createList());
}

Value* primPrint(List* list) {
    Value* info = first(list);
    printValue(info); printf("\n");
    return info;
}

Value* primAnd(List* list) {
    Value* lhs = first(list);
    Value* rhs = first(rest(list)->listval);
    if (lhs->type == AS_BOOL && rhs->type == AS_BOOL)
        return makeBoolVal(lhs && rhs );
    return makeListVal(createList());
}

Value* primOr(List* list) {
    Value* lhs = first(list);
    Value* rhs = first(rest(list)->listval);
    if (lhs->type == AS_BOOL && rhs->type == AS_BOOL)
        return makeBoolVal(lhs->intval || rhs->intval);
    return makeListVal(createList());
}

Value* primNot(List* list) {
    Value* lhs = first(list);
    if (lhs->type == AS_BOOL)
        return makeBoolVal(!lhs->boolval);
    return makeListVal(createList());
}

Value* primCons(List* list) {
    Value* val = list->head->info;
    Value* into = list->head->next->info;

    List* nl = createList();
    nl->head = makeListNode(val);
    if (into->type == AS_LIST) {
        nl->head->next = into->listval->head;
        nl->count = 1 + into->listval->count;
        nl->tail = into->listval->tail;
    } else {
        nl->head->next = makeListNode(into);
        nl->tail = nl->head->next;
        nl->count = 2;
    }
    return makeListVal(nl);
}

Value* primId(List* list) {
    Value* item = first(list);
    char *TypeLabels[6] = {"num", "symbol","bool","binding","func", "list"};
    return makeStringVal(makeString(TypeLabels[item->type], strlen(TypeLabels[item->type])));
}