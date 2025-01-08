#ifndef list_h
#define list_h
#include "value.h"
#include <stdbool.h>

int       listSize(List* list);
int       findList(List* list, Value* value);
bool      listEmpty(List* list);
void      printList(List* list);
void      freeList(List* list);
List*     createList();
List*     pushList(List* list, Value* value);
List*     appendList(List* list, Value* value);
List*     deleteNth(List* list, int N);
List*     copyList(List* list);
List*     copyOmitNth(List* list, int N);
List*     addMissing(List* dest, List* src);
Value*    getFromList(List* list, int N);
Value*    first(List* list);
Value*    rest(List* list);
listnode* makeListNode(Value* value);

#endif

