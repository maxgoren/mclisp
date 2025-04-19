#include <stdio.h>
#include "primitives.h"
#include "string.h"
#include "atom.h"
#include "list.h"
#include "eval.h"

Atom* primPlus(List* list) {
    return applyMathPrim('+', list);
}

Atom* primMinus(List* list) {
    return applyMathPrim('-', list);
}

Atom* primMul(List* list) {
    return applyMathPrim('*', list);
}

Atom* primDiv(List* list) {
    return applyMathPrim('/', list);
}

Atom* primMod(List* list) {
    return applyMathPrim('%', list);
}

Atom* primCar(List* list) {
    return first(list->head->info->listval);
}

Atom* primCdr(List* list) {
    return rest(list->head->info->listval);
}

Atom* primEqual(List* list) {
    Atom* lhs = first(list);
    Atom* rhs = first(rest(list)->listval);
    return makeBoolAtom(compareValue(lhs, rhs));
}

Atom* primLess(List* list) {
    Atom* lhs = first(list);
    Atom* rhs = first(rest(list)->listval);
    if (lhs->type == AS_NUM && rhs->type == AS_NUM)
        return makeBoolAtom(lhs->intval < rhs->intval);
    return makeListAtom(createList());
}

Atom* primGreater(List* list) {
    Atom* lhs = first(list);
    Atom* rhs = first(rest(list)->listval);
    if (lhs->type == AS_NUM && rhs->type == AS_NUM)
        return makeBoolAtom(lhs->intval > rhs->intval);
    return makeListAtom(createList());
}

Atom* primLessEq(List* list) {
    Atom* lhs = first(list);
    Atom* rhs = first(rest(list)->listval);
    if (lhs->type == AS_NUM && rhs->type == AS_NUM)
        return makeBoolAtom(lhs->intval <= rhs->intval);
    return makeListAtom(createList());
}

Atom* primGreaterEq(List* list) {
    Atom* lhs = first(list);
    Atom* rhs = first(rest(list)->listval);
    if (lhs->type == AS_NUM && rhs->type == AS_NUM)
        return makeBoolAtom(lhs->intval >= rhs->intval);
    return makeListAtom(createList());
}

Atom* primPrint(List* list) {
    Atom* info = first(list);
    printValue(info); printf("\n");
    return info;
}

Atom* primAnd(List* list) {
    Atom* lhs = first(list);
    Atom* rhs = first(rest(list)->listval);
    if (lhs->type == AS_BOOL && rhs->type == AS_BOOL)
        return makeBoolAtom(lhs && rhs );
    return makeListAtom(createList());
}

Atom* primOr(List* list) {
    Atom* lhs = first(list);
    Atom* rhs = first(rest(list)->listval);
    if (lhs->type == AS_BOOL && rhs->type == AS_BOOL)
        return makeBoolAtom(lhs->intval || rhs->intval);
    return makeListAtom(createList());
}

Atom* primNot(List* list) {
    Atom* lhs = first(list);
    if (lhs->type == AS_BOOL)
        return makeBoolAtom(!lhs->boolval);
    return makeListAtom(createList());
}

Atom* primCons(List* list) {
    Atom* val = first(list);
    Atom* into = list->head->next->info;

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
    return makeListAtom(nl);
}

Atom* primAppend(List* list) {
    Atom* pri = list->head->info;
    Atom* sec = list->head->next->info;
    if (sec->type != AS_LIST) {
        return makeListAtom(createList());
    }
    sec->listval = appendList(sec->listval, pri);
    return sec;
}

Atom* primId(List* list) {
    Atom* item = first(list);
    char *TypeLabels[6] = {"num", "symbol","bool","binding","func", "list"};
    return makeSymbolAtom(makeString(TypeLabels[item->type], strlen(TypeLabels[item->type])));
}

Atom* primMin(List* list) {
    return applyMathPrim('<', list);
}

Atom* primMax(List* list) {
    return applyMathPrim('>', list);
}

Atom* primSymbolicEquality(List* args) {
    Atom* va = first(args);
    Atom* vb = first(rest(args)->listval);
    if (va->type == AS_SYMBOL && vb->type == AS_SYMBOL)
        return makeBoolAtom(compareValue(va, vb));
    return makeListAtom(createList());
}

Atom* primList(List* args) {
    List* reso = createList();
    List* env = createList();
    for (listnode* it = args->head; it != NULL; it = it->next) {
        reso = appendList(reso, it->info);
    }
    return makeListAtom(reso);
}

//This flattens lists as it joins them
Atom* primJoin(List* list) {
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
    return makeListAtom(reso);
}

Atom* primApply(List* list) {
    Atom* func = first(list);
    Atom* args = first(rest(list)->listval);
    if (func->type != AS_FUNCTION || args->type != AS_LIST) {
        printf("Error: Apply Expects a function and a list for arguments.\n");
        return makeListAtom(createList());
    }
    return apply(func->funcval, args->listval, createList());
}