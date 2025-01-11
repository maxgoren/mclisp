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

Value* primMod(List* list) {
    return applyMathPrim('%', list);
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

Value* primAppend(List* list) {
    Value* pri = list->head->info;
    Value* sec = list->head->next->info;
    if (sec->type != AS_LIST) {
        return makeListVal(createList());
    }
    sec->listval = appendList(sec->listval, pri);
    return sec;
}

Value* primId(List* list) {
    Value* item = first(list);
    char *TypeLabels[6] = {"num", "symbol","bool","binding","func", "list"};
    return makeStringVal(makeString(TypeLabels[item->type], strlen(TypeLabels[item->type])));
}

Value* primMin(List* list) {
    return applyMathPrim('<', list);
}

Value* primMax(List* list) {
    return applyMathPrim('>', list);
}

Value* primSymbolicEquality(List* args) {
    Value* va = first(args);
    Value* vb = first(rest(args)->listval);
    if (va->type == AS_SYMBOL && vb->type == AS_SYMBOL)
        return makeBoolVal(compareValue(va, vb));
    return makeListVal(createList());
}

Value* primList(List* args) {
    List* reso = createList();
    List* env = createList();
    for (listnode* it = args->head; it != NULL; it = it->next) {
        reso = appendList(reso, it->info);
    }
    return makeListVal(reso);
}

Value* primJoin(List* list) {
    List* reso = createList();
    List* env = createList();
    for (listnode* it = list->head; it != NULL; it = it->next) {
        if (it->info->type == AS_LIST) {
            for (listnode* iit = it->info->listval->head; iit != NULL; iit = iit->next) {
                reso = appendList(reso, iit->info);
            }
        } else {
            reso = appendList(reso, it->info);
        }
    }
    return makeListVal(reso);
}