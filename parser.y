%{
#include <stdio.h>
#include <stdlib.h>
void yyerror(char *err);
%}

%token IF ELSE
%token WHILE FOR DO
%token ID OP LIT TYPE KEY ASSGN

%%
prog: stmts { printf("Compiled!\n"); }
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
     ;

/* Declaration of variables. */
declr: TYPE params
     ;

fdeclr: TYPE ID '(' fparams ')' '{' stmts '}'
      | TYPE ID '(' ')' '{' stmts '}'
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

fcall: ID '(' params ')'
     ;

params: param ',' params
      | param
      ;
param: ID
     | LIT
     | ID '[' LIT ']'
     ;

fparams: fparam ',' fparams
       | fparam
       ;
fparam: TYPE ID '[' LIT ']'
      | TYPE ID '[' ']'
      | TYPE ID
      ;


%%

void yyerror(char *err)
{
    fprintf(stderr, "Error: %s\n", err);
}

int main()
{
    yyparse();
    return 0;
}
