#ifndef parse_h
#define parse_h
#include "atom.h"
#include "list.h"


Atom* parseSymbol(char buff[], int i);
List* stringToList(char* buff);

#endif