#include "symbol.h"
#include <unistd.h>

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
    
    if (func) {
        tmp_->finfo = malloc (sizeof(Func));
        tmp_->finfo->retType = type;
        int *args, i, r;
        args = malloc(sizeof(int)*32);
        i = 0;
        while ((r = popFromStack(t)) != -1) {
            args[i++] = r;
        }
        tmp_->finfo->args = args;
        tmp_->finfo->n = i;
    }

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

Entry*
getEntryByName(Table *t, char *key)
{
    Entry *tmp = t->head;
    if (tmp == NULL)
        return NULL;

    while (tmp != NULL) {
        if (strcmp(tmp->key, key) == 0)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
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

    // Make an interactive shell to inspect.
    char buf[32];
    memset(buf, 0, sizeof(buf));
    char *ptr = buf;
    printf("\n");
    printf (">> ");
    fflush(stdout);
    int n = read (0, buf, sizeof(buf));
    buf[n-1] = '\0';
    while (buf[0] != 'e') {
        inspect (&t, ++ptr);
        printf ("\n>> ");
        fflush(stdout);
        memset(buf, 0, sizeof(buf));
        n = read (0, buf, sizeof(buf));
        buf[n-1] = '\0';
        ptr = buf;
    }
}

void
inspect(Table *t, char *buf)
{
    char ident[32];
    int i = 0;
    memset(ident, 0, sizeof(ident));

    buf = buf + 1;
    while (*buf != '\0') {
        if (*buf == ' ') {
            buf++;
            continue;
        }
        ident[i] = *buf;
        buf++;
        i++;
    }

    ident[i] = '\0';
    Entry *e = getEntryByName (t, ident);
    if (e) {
        printf ("%s:\n", e->key);
        printf ("DataType/ReturnType: %d\n", 1);
        if (e->function) {
            printf ("Number of arguments: %d\n", e->finfo->n);
            printf ("Function Signature: %s\n", makeSig (e->finfo->n, e->finfo->args));
        }
    } else {
        printf ("%s not found in the symbol table\n", ident);
    }
}

char *
makeSig (int n, int *args)
{
    int i = 0;
    char *buf;
    buf = malloc (32);
    for (i = 0; i < n; i++) {
        strncat(buf, intToType (*args), sizeof(buf));
        strncat(buf, ", ", 1);
        args++;
    }

    return buf;
}

char *
intToType(int n)
{
    char *buf;
    buf = malloc (32);
    switch (n) {
        case VOID:
            strncpy (buf, "void", sizeof(buf));
            break;                          
        case INT:                           
            strncpy (buf, "int", sizeof(buf));
            break;                          
        case CHAR:                          
            strncpy (buf, "char", sizeof(buf));
            break;                          
        case FLOAT:                         
            strncpy (buf, "float", sizeof(buf));
            break;                           
        case DOUBLE:                         
            strncpy (buf, "double", sizeof(buf));
            break;
    }
    return buf;
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

void
pushToStack(Table *t, int type)
{
    if (t->ptr == NULL)
        t->ptr = &(t->stack[0]);
    *(t->ptr) = type;
    t->ptr++;
}

int
popFromStack(Table *t)
{
    if (&(t->stack[0]) == t->ptr)
        return -1;

    int ret;
    t->ptr--;
    ret = *(t->ptr);
    return ret;
}

int
checkCall(Table *t, char *ident)
{
    Entry *e = getEntryByName(t, ident);
    
    if (!e)
        return 0;

    int *args, i, r;
    args = malloc(sizeof(int)*32);
    i = 0;
    while ((r = popFromStack(t)) != -1) {
        args[i++] = r;
    }

    int j = 0;
    if (i != e->finfo->n)
        return 0;

    printf ("hello");
    for (; j < i; j++) {
        if (args[j] != e->finfo->args[j])
            return 0;
    }

    return 1;
}

