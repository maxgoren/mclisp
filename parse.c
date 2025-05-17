#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "parse.h"
#include "atom.h"
#include "list.h"
#include "eval.h"
#include "primitives.h"
#include "specialform.h"
#include <string.h>

bool shouldSkip(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

bool isSpecialChar(char c) {
    return (c == '-' || c == '?' || c == '+' || c == '!' || c == '*' ||
            c == '/' || c == '\'' || c == '&' || c == '%' || c == ':');
}

Atom* parseSymbol(char buff[], int* i) {
    int k = *i;
    while (isalpha(buff[k]) || isSpecialChar(buff[k]))
        k++;
    int len = k - *i;
    char* sub = strndup(buff+*i, len);
    *i += len;
    return makeSymbolAtom(makeString(sub, len));
}

Atom* parseNumber(char buff[], int* i) {
    int val = 0, j = *i;
    while (isdigit(buff[j])) {
        val = 10 * val + (buff[j++]-'0');
    }
    *i=j;
    return makeIntAtom(val);
}


Atom* parseString(char buff[], int* i) {
    int k = *i+1, len = 0;;
    while (buff[k] != '\0' && buff[k] != '"')
        k++;
    if (buff[k] != '"') {
        printf("Error: Unterminated string.\n");
        return makeNil();
    } else {
        k++;
    }
    len = k-*i;
    char* sub = strndup(buff+*i, len);
    *i += len;
    return makeStringAtom(makeString(sub, len));
}

List* addToList(List* addTo, Atom* item, bool isquoted) {
    if (isquoted) {
        List* nl = createList();
        nl = appendList(nl, makeSymbolAtom(makeString("'", 1)));
        nl = appendList(nl, item);
        item = makeListAtom(nl);
    }
    addTo = appendList(addTo, item);
    return addTo;
}


int t = 0;
List* stringToList(char* buff) {
    List* result = createList();
    int i = 0;
    bool quoted = false;
    if (buff[i] == '(') { i++; }
    for (; buff[i];) {
        if (shouldSkip(buff[i])) {
            i++; continue;
        }
        if (buff[i] == '\'') {
            quoted = true; i++;
        }
        if (isalpha(buff[i]) || isSpecialChar(buff[i])) {
            result = addToList(result, parseSymbol(buff, &i), quoted);
        } else if (isdigit(buff[i])) {
            result = addToList(result, parseNumber(buff, &i), quoted);
        } else if (buff[i] == '"') {
            result = addToList(result, parseString(buff, &i), quoted);
        } else if (buff[i] == '(') {
            result = addToList(result, makeListAtom(stringToList(buff+i)), quoted);
            i = i+t+1;
        } else if (buff[i] == ')') {
            t = i;
            return result;
        }
        quoted = false;
    }
    return result;
}