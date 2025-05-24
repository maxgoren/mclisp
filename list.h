#ifndef list_h
#define list_h
#include "atom.h"
#include <stdbool.h>

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
#define NOT_FOUND -1

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
List*     mergeUnique(List* dest, List* src);
Atom*    getFromList(List* list, int N);
Atom*    first(List* list);
Atom*    second(List* list);
Atom*    rest(List* list);
listnode* makeListNode(Atom* value);

#endif

