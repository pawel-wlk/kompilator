%option noyywrap
%option yylineno
%{

#include "../src/Variable.hpp"
#include "../src/Value.hpp"
#include "parser.tab.hpp"

#include <string>
#include <iostream>

int yylex();
%}

PUNCT                   [\(\)\:\;]

%x COMMENT

%%
"DECLARE"               { return DECLARE; }
"BEGIN"                 { return T_BEGIN; }
"END"                   { return END; }

"READ"                  { return READ; }
"WRITE"                 { return WRITE; }

"IF"                    { return IF; }
"THEN"                  { return THEN; }
"ELSE"                  { return ELSE; }
"ENDIF"                 { return ENDIF; }

"WHILE"                 { return WHILE; }
"DO"                    { return DO; }
"ENDWHILE"              { return ENDWHILE; }
"ENDDO"                 { return ENDDO; }

"FOR"                   { return FOR; }
"FROM"                  { return FROM; }
"TO"                    { return TO; }
"ENDFOR"                { return ENDFOR; }

"ASSIGN"                { return ASSIGN; }

"EQ"                    { return EQ; }
"NEQ"                   { return NEQ; }
"LE"                    { return LE; }
"GE"                    { return GE; }
"LEQ"                   { return LEQ; }
"GEQ"                   { return GEQ; }

"PLUS"                  { return PLUS; }
"MINUS"                 { return MINUS; }
"DIV"                   { return DIV; }
"MOD"                   { return MOD; }
"TIMES"                 { return TIMES; }
";"                     { return SEMICOLON; }
":"                     { return COLON; }
","                     { return COMMA; }
"("                     { return LPAREN; }
")"                     { return RPAREN; }
[_a-z]+                 { yylval.pid = yytext; return PID; }
[0-9]+                  { yylval.num = std::stoll(yytext); return NUM; }
\[.*\]                  
"["                     { BEGIN(COMMENT); }
<COMMENT>{
    [^\]]               ;
    "]"                 BEGIN(INITIAL);
}

[ \t\r\n]+              ; 
.                       { return ERROR; }
%%