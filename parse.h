#ifndef parse_h
#define parse_h
#include "atom.h"
#include "list.h"

Atom* parseString(char buff[], int* i);
Atom* parseSymbol(char buff[], int* i);
Atom* parseNumber(char buff[], int* i);
List* stringToList(char* buff);

#endif