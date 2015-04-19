#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VOID   0
#define INT    1
#define CHAR   2
#define FLOAT  3
#define DOUBLE 4

typedef struct entry {
    char key[60];         // Maximum length of an identifier.
    int function;         // 1 = function, 0 = otherwise.
    int type;             // Data type of the identifier. If the identifier is a function, then this referes to the return type instead.
    int exists;           // 1 = exists, 0 = deleted (out of scope).
    struct entry *next;   // Pointer to next entry in the table.
    int lineno;           // Line number of this particular entry.
    struct func *finfo;   // To store function information. Used only if function is set to 1.
} Entry;

typedef struct func {
    int n;                // Number of parameters required for the function call.
    int *args;            // List of types of arguments the function takes.
    int retType;          // Function return type.
    int scope[2];         // Scope of this function, expressed as a pair of starting and ending lineno.
} Func;


typedef struct table {
    Entry *head;
    int stack[32];
    int *ptr;
} Table;

typedef struct frame {
    Table *t;
} Frame;

void insert(Table *t, char *key, int type, int func);
int exists(Table *t, char *key);
void applyType(Table *t, int type);

// print Functions
void printTable(Table t);
void inspect (Table *t, char *buf);
char* intToType (int n);
char* makeSig (int n, int *args);


int createDataType(char *type);

void pushToStack(Table *t, int type);
int popFromStack(Table *t);

int checkCall(Table *t, char *ident);
