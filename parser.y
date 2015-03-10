%{
#include <stdio.h>
#include <stdlib.h>
#include "symbol.h"

void yyerror(char *err);

Table t;
int trial;
void insertIdent(char *key, int type, int isFunc);
%}

%union {
    int dtype;
    struct entry *e;
    struct frame *f;
}

%token IF ELSE
%token WHILE FOR DO
%token OP KEY ASSGN
%token <e> ID LIT
%token <dtype> TYPE
%type <e> param params

%%
prog: stmts { 
              printf("Compiled!\n"); 
              printTable(t); 
            }
    ;


stmts: stmt
     ;

/* Basic statement.
 * Basic statement can be an expression (arithmetic or logical), declaration, conditional statement, looping statement.
 */
stmt: expr ';' stmt
    | cond stmt 
    | loop stmt
    | declr ';' stmt /* Function and variables. */
    | assgn ';' stmt
    | fcall ';' stmt /* Function call. */
    | fdeclr stmt
    | KEY LIT ';' stmt
    |
    ;

/* Expressions. */
expr: ID OP ID
    | ID OP LIT
    | LIT OP LIT
    | LIT OP ID
    | expr OP expr
    | '(' expr ')'
    | LIT
    | ID
    |
    ;

assgn: ID ASSGN expr 
                     {
                          if (!exists(&t, $1->key)) {
                              char buf[100];
                              snprintf (buf, 100, "Undeclared reference to identifier %s", $1->key);
                              yyerror(buf);
                              exit(0);
                          }
                     }
     ;

/* Declaration of variables. */
declr: TYPE params {
                        applyType(&t, $1);
                   }
     ;

fdeclr: TYPE ID '(' fparams ')' '{' stmts '}'
      { insertIdent($2->key, $1, 1); }
      | TYPE ID '(' ')' '{' stmts '}'
      { insertIdent($2->key, $1, 1); }
      ;

/* Conditionals. if, else if, else. */
cond: IF '(' expr ')' '{' stmts '}'
    | ELSE '{' expr '}'
    | ELSE IF '(' expr ')' '{' stmts '}'
    ;

/* looping constructs.*/
loop: FOR '(' assgn ';' expr ';' assgn ')' '{' stmts '}'
    | WHILE '(' expr ')' '{' stmts '}'
    | DO '{' stmts '}' WHILE '(' expr ')' ';'
    ;

fcall: ID '(' aparams ')'
     ;

params: param ',' params
      | param
      ;
param: ID { insertIdent($1->key, -1, 0); }
     | LIT
     | ID '[' LIT ']'
     ;
aparams: aparam ',' aparams
      | aparam
      ;
aparam: ID
     | LIT
     | ID '[' LIT ']'
     ;

fparams: fparam ',' fparams
       | fparam
       ;
fparam: TYPE ID '[' LIT ']' { insertIdent($2->key, $1, 0); }
      | TYPE ID '[' ']'     { insertIdent($2->key, $1, 0); }
      | TYPE ID             { insertIdent($2->key, $1, 0); }
      ;


%%

void yyerror(char *err)
{
    fprintf(stderr, "Error: %s\n", err);
}

void insertIdent(char *key, int type, int isFunc)
{
    if (exists(&t, key)) {
          yyerror("Redeclaration of identifier");
          exit(0);
    }
    insert(&t, key, type, isFunc); 
}

int main(int argc, char *argv[])
{
    extern int yydebug;
    yydebug = 0;
    if (argc > 1) {
        yydebug = atoi(argv[1]);
    }
    yyparse();
    return 0;
}
