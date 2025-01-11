#ifndef value_h
#define value_h
#include <stdbool.h>
#include <stdlib.h>

enum ValueType {
    AS_NUM, AS_SYMBOL, AS_BOOL, AS_BINDING, AS_FUNCTION, AS_LIST, AS_SF
};

typedef struct List List;
typedef struct Function Function;
typedef struct Binding Binding;
typedef struct String String;
typedef struct SpecialForm SpecialForm;
typedef struct {
    enum ValueType type;
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
} Value;


typedef struct listnode_ {
    Value* info;
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
    Value* symbol;
    Value* value;
} Binding;

enum funcType {
    PRIMITIVE, LAMBDA
};

typedef struct Function {
    enum funcType type;
    List* env;
    List* freeVars;
    Value* (*func)(List*);
    Value* code;
} Function;

#define NO_EVAL  0
#define EVAL  1

typedef struct SpecialForm {
    String* name;
    int numArgs;
    char flags[3];
    Value* (*func)(List*, List*);
} SpecialForm;

typedef struct GC_ {
    List* objList;
} GC;

List* init(List* env);
void printValue(Value* val);
bool compareValue(Value* lhs, Value* rhs);
bool compareString(String* lhs, String* rhs);
bool compareList(List* lhs, List* rhs);
bool compareBinding(Binding* lhs, Binding* rhs);
String* makeString(char* str, int len);
Binding* makeBinding(Value* symbol, Value* value);
Value* makeIntVal(int value);
Value* makeStringVal(String* val);
Value* makeListVal(List* list);
Value* makeBoolVal(bool value);
Value* makeBindingVal(Binding* binding);
Value* makeFunctionValue(Function* function);
Function* makePrimitveFunction(Value* (*func)(List*));
Function* makeLambdaFunction(List* vars, List* code, List* env);
List* bindSymbolToValue(List*,Binding*);
List* makePrim(List*,String*,Value* (func)(List*));
void freeValue(Value* value);
void freeBinding(Binding* binding);
void freeString(String* str);

#define WHITE  0
#define GREY   1

extern GC* gc;

void initGC();
void registerObject(GC* gc, Value* val);
List* mark(List* env);
List* sweep(List* env);

#endif