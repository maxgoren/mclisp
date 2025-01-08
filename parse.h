#ifndef parse_h
#define parse_h
#include "value.h"
#include "list.h"


Value* parseSymbol(char buff[], int i);
List* stringToList(char* buff);

#endif