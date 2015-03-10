#include "symbol.h"

void
insert(Table *t, char *key, int type, int func)
{
    Entry *tmp = t->head;
    if (tmp == NULL) {
        tmp = (Entry *) malloc(sizeof(Entry));
        strncpy (tmp->key, key, sizeof(tmp->key));
        tmp->type = type;
        tmp->function = func;
        tmp->exists = 1;
        tmp->next = NULL;
        t->head = tmp;
        return;
    }

    while (tmp->next != NULL)
        tmp = tmp->next;

    Entry *tmp_;
    tmp_ = (Entry *) malloc(sizeof(Entry));
    strncpy (tmp_->key, key, sizeof(tmp->key));
    tmp_->type = type;
    tmp_->function = func;
    tmp_->exists = 1;
    tmp_->next = NULL;
    tmp->next = tmp_;

}

int
exists(Table *t, char *key)
{
    Entry *tmp = t->head;
    if (tmp == NULL)
        return 0;

    while (tmp != NULL) {
        if (strcmp(tmp->key, key) == 0)
            return 1;
        tmp = tmp->next;
    }

    return 0;
}

void
applyType(Table *t, int type)
{
    Entry *tmp = t->head;
    if (tmp == NULL)
        return;

    while (tmp != NULL) {
        if (tmp->type == -1)
            tmp->type = type;

        tmp = tmp->next;
    }
}


void 
printTable(Table t)
{
    Entry *e = t.head;

    printf("\nSymbol Table:\n"
           " _________________________________________________________ \n"
           "|                      |                      |           |\n"
           "| %-20s | %-20s | %-9s |\n"
           "|----------------------|----------------------|-----------|\n"
           , "Identifier", "DataType/ReturnType", "Function?");

    while (e != NULL) {
        printf("| %-20s | %-20d | %-9s |\n", e->key, e->type, e->function?"Yes":"No");
        e = e->next;
    }
    printf("|______________________|______________________|___________|\n");

}

int
createDataType(char *type)
{
    if (strcmp(type, "int") == 0)
        return INT;
    if (strcmp(type, "char") == 0)
        return CHAR;
    if (strcmp(type, "float") == 0)
        return FLOAT;
    if (strcmp(type, "void") == 0)
        return VOID;
    if (strcmp(type, "double") == 0)
        return DOUBLE;
}
    
