#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VOID   0
#define INT    1
#define CHAR   2
#define FLOAT  3
#define DOUBLE 4

typedef struct entry {
    char key[60]; // Maximum length of an identifier.
    int function; // 1 = function, 0 = otherwise.
    int type;     // Data type of the identifier. If the identifier is a function, then this referes to the return type instead.
    int exists;   // 1 = exists, 0 = deleted (out of scope).
    struct entry *next;  // Pointer to next entry in the table.
} Entry;

typedef struct table {
    Entry *head;
} Table;

typedef struct frame {
    Table *t;
} Frame;

//typedef struct datatype {
    //int type;
//} DataType;

void insert(Table *t, char *key, int type, int func);
int exists(Table *t, char *key);
void printTable(Table t);
void applyType(Table *t, int type);

int createDataType(char *type);
