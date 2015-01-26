D [0-9]
a [a-z]
A [A-Z]
IDENT [a-zA-Z_]+[a-zA-Z_0-9]*
PUNCTUATOR [\[\]\(\)\{\}\.(\->)(++)(\-\-)&*+\-~!/%(<<)(>>)<>(<=)(>=)(==)(!=)\^\|(&&)(\|\|);,#]
KEYWORD (auto)|(break)|(case)|(char)|(const)|(continue)|(default)|(do)|(double)|(else)|(enum)|(extern)|(float)|(for)|(goto)|(if)|(inline)|(int)|(long)|(register)|(restrict)|(return)|(short)|(signed)|(sizeof)|(static)|(struct)|(switch)|(typedef)|(union)|(unsigned)|(void)|(volatile)|(while)|(_Bool)|(_Complex)|(_Imaginary)
%x STRING
%x COMMENT
%{

char* subs(char *s, int n)
{
    char *s1;
    s1 = (char *)malloc(n);
    memcpy(s1, s, n);
    return s1;
}

%}

%%
["]                   { BEGIN STRING; yymore(); }

<STRING>\"            { 
                          printf("<%s, LITERAL>\n", yytext);
                          BEGIN INITIAL;
                      }
<STRING>[^"]          { yymore(); }
<STRING>"\\\""        { yymore(); }

<COMMENT>"\n"         ;
<COMMENT>[^(*/)]*$    ;
<COMMENT>.*"\n"       { BEGIN INITIAL; }

"/*"                  { BEGIN COMMENT; }
"//".*                ;
^[ ]*                 ;
{KEYWORD}[ ]          { printf("<%s, KEYWORD>\n", subs(yytext, yyleng-1)); }
{PUNCTUATOR}          { printf("<%s, PUNCTUATOR> \n", yytext); }
{IDENT}               { printf("<%s, IDENTIFIER> \n", yytext); }
{D}*                  { printf("<%s, LITERAL> \n", yytext); }
\n                    ;
%%

int main()
{
    yylex();
    return 0;
}

