%option noyywrap
/* Documentation for the scanner.
 * 
 * LEGEND:
 * -------
 * 
 *  
 */

D [0-9]
a [a-z]
A [A-Z]
IDENT [a-zA-Z_]+[a-zA-Z_0-9]*
PUNCTUATOR [\[\]\(\)\{\}\.(\->)(++)(\-\-)&*+\-~!/%(<<)(>>)<>(<=)(>=)(==)(!=)\^\|(&&)(\|\|);,#]
KEYWORD "auto"|"break"|"case"|"const"|"continue"|"default"|"enum"|"extern"|"goto"|"inline"|"long"|"register"|"restrict"|"return"|"short"|"signed"|"sizeof"|"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"volatile"|"_Bool"|"_Complex"|"_Imaginary"
BINOP [-+*/&\|^]|"<="|">="|"=="|"&&"|"||"|"!="
ASSGN "="|"+="|"-="|"*="|"/="
TYPE (int)|(char)|(float)|(double)|(void)
%x STRING
%x COMMENT
%{
#include "y.tab.h"
#include "symbol.h"
#include <string.h>
char* subs(char *s, int n)
{
    char *s1;
    s1 = (char *)malloc(n);
    memcpy(s1, s, n);
    return s1;
}

%}

%%


"/*"                  { BEGIN COMMENT; } /* Comment section */
"//".*                ;
<COMMENT>"\n"         ;
<COMMENT>[^(*/)]*     ;
<COMMENT>.*"\n"       { BEGIN INITIAL; }

["]                   { BEGIN STRING; yymore(); } /* String section */
<STRING>\"            { 
                          BEGIN INITIAL;
                          return LIT;
                      }
<STRING>[^"]          { yymore(); }
<STRING>"\\\""        { yymore(); }


"if"                  { return IF; }
"else"                { return ELSE; }
"while"               { return WHILE; }
"for"                 { return FOR; }
"do"                  { return DO; }

{KEYWORD}             { return KEY; }
{BINOP}               { return OP; }

{TYPE}                { 
                        yylval.dtype = createDataType(yytext);
                        return TYPE; 
                      }

{IDENT}               { 
                        yylval.e = malloc(sizeof(Entry));
                        strncpy (yylval.e->key, yytext, sizeof(yylval.e->key));
                        yylval.e->exists = 1;
                        yylval.e->next = NULL;
                        return ID;
                      }
{D}+"."{D}+           { return LIT; }
{D}+                  { return LIT; }
\n                    ;
"#".*                 ;
" "                   ;
{ASSGN}               { return ASSGN; }
.                     { return *yytext; }
%%
