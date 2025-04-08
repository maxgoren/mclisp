#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "atom.h"

int listSize(List* list) {
    return list == NULL ? -1:list->count;
}

bool listEmpty(List* list) {
    return list != NULL && list->head == NULL;
}

int findList(List* list, Atom* value) {
    int k = 0;
    for (listnode* it = list->head; it != NULL; it = it->next) {
        if (compareValue(value, it->info))
            return k;
        k++;
    }
    return -1;
}

List* createList() {
    List* nl = malloc(sizeof(List));
    nl->head = NULL;
    nl->tail = NULL;
    nl->count = 0;
    return nl;
}

List* pushList(List* list, Atom* value) {
    listnode* nl = makeListNode(value);
    nl->next = list->head;
    list->head = nl;
    if (list->tail == NULL) list->tail = nl;
    list->count += 1;
    return list;
}

List* appendList(List* list, Atom* value) {
    listnode* ln = makeListNode(value);
    if (list->head == NULL) {
        list->head = ln;
    } else {
        list->tail->next = ln;
    }
    list->tail = ln;
    list->count += 1;
    return list;
}

listnode* makeListNode(Atom* value) {
    listnode* ln = malloc(sizeof(listnode));
    ln->info = value;
    ln->next = NULL;
    return ln;
}

List* copyList(List* list) {
    List* nl = createList();
    for (listnode* it = list->head; it != NULL; it = it->next)
        nl = appendList(nl, it->info);
    return nl;
}

List* copyOmitNth(List* list, int N) {
    List* nl = createList();
    int i = 0;
    for (listnode* it = list->head; it != NULL; it = it->next) {
        if (i != N)
            nl = appendList(nl, it->info);
        i++;
    }
    return nl;
}

List* deleteNth(List* list, int N) {
    List* nl = copyOmitNth(list, N);
    return nl;
}

List* addMissing(List* dest, List* src) {
    for (listnode* it = src->head; it != NULL; it = it->next) {
        if (findList(dest, it->info) == -1)
            dest = appendList(dest, it->info);
    }
    return dest;
}

Atom* getFromList(List* list, int N) {
    int i = 0;
    listnode* x = list->head;
    while (x != NULL && i < N) {
        x = x->next;
        i++;
    }
    return x == NULL ? makeListVal(createList()):x->info;
}

Atom* first(List* list) {
    return (list == NULL || list->head == NULL) ? makeListVal(createList()):list->head->info;
}

Atom* rest(List* list) {
    return makeListVal(copyOmitNth(list, 0));
}

void printList(List* list) {
    printf("(");
    for (listnode* it = list->head; it != NULL; it = it->next) {
        printValue(it->info);
        printf(" ");
    }
    printf(")");
}

void freeList(List* list) {
    if (list == NULL)
        return;
    listnode* it = list->head; 
    while (it != NULL) {
        listnode* x = it;
        it = it->next;
        freeValue(x->info);
        free(x);
    }
    free(list);
}