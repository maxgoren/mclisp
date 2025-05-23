#ifndef value_h
#define value_h
#include <stdbool.h>
#include <stdlib.h>

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

enum AtomType {
    AS_NUM, AS_SYMBOL, AS_BOOL, AS_BINDING, AS_FUNCTION, AS_LIST, 
    AS_CELL, AS_SF, AS_NIL, AS_ERROR, AS_STRING
};

typedef struct List List;
typedef struct Function Function;
typedef struct Binding Binding;
typedef struct String String;
typedef struct SpecialForm SpecialForm;
typedef struct {
    enum AtomType type;
    union {
        int intval;
        bool boolval;
        List* listval;
        Function* funcval;
        Binding* bindval;
        String* stringval;
        SpecialForm* sf;
    };
    int mark;
} Atom;


typedef struct listnode_ {
    Atom* info;
    struct listnode_* next;
} listnode;

typedef struct List {
    listnode* head;
    listnode* tail;
    int count;
} List;

typedef struct String {
    char* data;
    int len;
} String;

typedef struct Binding {
    Atom* symbol;
    Atom* value;
} Binding;

enum funcType {
    PRIMITIVE, LAMBDA
};

typedef struct Function {
    enum funcType type;
    List* env;
    List* freeVars;
    Atom* (*func)(List*);
    Atom* code;
} Function;

#define NO_EVAL  0
#define EVAL  1

typedef struct SpecialForm {
    String* name;
    int numArgs;
    char flags[3];
    Atom* (*func)(List*, List*);
} SpecialForm;

typedef struct GC_ {
    List* objList;
} GC;


enum AtomType typeOf(Atom* val);
List* init(List* env);
void printValue(Atom* val);
bool compareValue(Atom* lhs, Atom* rhs);
bool compareString(String* lhs, String* rhs);
bool compareList(List* lhs, List* rhs);
bool compareBinding(Binding* lhs, Binding* rhs);
String* makeString(char* str, int len);
Binding* makeBinding(Atom* symbol, Atom* value);
Atom* makeIntAtom(int value);
Atom* makeSymbolAtom(String* val);
Atom* makeStringAtom(String* val);
Atom* makeListAtom(List* list);
Atom* makeBoolAtom(bool value);
Atom* makeBindingAtom(Binding* binding);
Atom* makeFunctionAtom(Function* function);
Atom* makeNil();
Function* makePrimitveFunction(Atom* (*func)(List*));
Function* makeLambdaFunction(List* vars, List* code, List* env);
List* addBindingToEnvironment(List*,Binding*);
List* createPrimitive(List*,String*,Atom* (func)(List*));
void freeValue(Atom* value);
void freeBinding(Binding* binding);
void freeString(String* str);

bool is_literal(Atom* val);
bool is_symbol(Atom* val);
bool is_list(Atom* val);
bool is_binding(Atom* val);
bool is_function(Atom* val);
bool is_primitive(Function* func);
bool is_lambda(Function* func);

#define WHITE  0
#define GREY   1
extern bool traceGC;
extern GC* gc;
extern Atom* NIL;
void initGC();
void registerObject(GC* gc, Atom* val);
List* mark(List* env);
List* sweep(List* env);
List* runGC(List* env);
#endif