#ifndef list_h
#define list_h
#include "value.h"
#include <stdbool.h>

int       listSize(List* list);
int       findList(List* list, Atom* value);
bool      listEmpty(List* list);
void      printList(List* list);
void      freeList(List* list);
List*     createList();
List*     pushList(List* list, Atom* value);
List*     appendList(List* list, Atom* value);
List*     deleteNth(List* list, int N);
List*     copyList(List* list);
List*     copyOmitNth(List* list, int N);
List*     addMissing(List* dest, List* src);
Atom*    getFromList(List* list, int N);
Atom*    first(List* list);
Atom*    rest(List* list);
listnode* makeListNode(Atom* value);

#endif

