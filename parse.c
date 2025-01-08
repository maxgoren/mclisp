#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "parse.h"
#include "value.h"
#include "list.h"
#include "eval.h"
#include "primitives.h"
#include "specialform.h"

bool shouldSkip(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

bool isSpecialChar(char c) {
    return (c == '-' ||c == '?' || c == '+' || c == '*' || c == '/' || c == '\'' || c == '&');
}

Value* parseSymbol(char buff[], int i) {
    int k = i;
    while (isalpha(buff[k]) || isSpecialChar(buff[k]))
        k++;
    char* sub = malloc(sizeof(char)*k+1);
    int t;
    for (t = 0; t < k-i; t++)
        sub[t] = buff[i+t];
    sub[t] = '\0';
    return makeStringVal(makeString(sub, t));
}

List* addToList(List* addTo, Value* item, bool isquoted) {
    if (isquoted) {
        List* nl = createList();
        nl = appendList(nl, makeStringVal(makeString("'", 1)));
        nl = appendList(nl, item);
        item = makeListVal(nl);
    }
    addTo = appendList(addTo, item);
    return addTo;
}

int t = 0;
List* parse(char* buff) {
    List* result = createList();
    int i = 0;
    bool quoted = false;
    if (buff[i] == '(') { i++; }
    for (; buff[i];) {
        if (shouldSkip(buff[i])) {
            i++;
            continue;
        }
        if (buff[i] == '\'') {
            quoted = true;
            i++;
        }

        if (isalpha(buff[i]) || isSpecialChar(buff[i])) {
            int m = i;
            Value* sym = parseSymbol(buff, i);
            result = addToList(result, sym, quoted);
            i = m + sym->stringval->len;
        } else if (isdigit(buff[i])) {
            int val = 0;
            while (isdigit(buff[i])) {
                val = 10 * val + (buff[i++]-'0');
            }
            result = addToList(result, makeIntVal(val), quoted);
        } else if (buff[i] == '(') {
            Value* sym = makeListVal(parse(buff+i));
            result = addToList(result, sym, quoted);
            i = i+t+1;
        } else if (buff[i] == ')') {
            t = i;
            return result;
        }
        quoted = false;
    }
    return result;
}

List* stringToList(char* buff) {
    return parse(buff);
}