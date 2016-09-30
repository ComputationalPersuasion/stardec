%option noyywrap
%option nounput

%{
#include "logicaloperator.h"
#include "parser.hpp"
#define YY_DECL extern "C" int yylex()
%}

LABEL [a-zA-Z0-9_]+

%%

"arg"   {return ARG;}
"att"   {return ATT;}
"goal"  {return GOAL;}
"AND"   {return AND;}
"OR"    {return OR;}
"NOT"   {return NOT;}

{LABEL} {yylval.str = strdup(yytext); return LABEL;}
[ \t\n] {}
"("     {return LEFTPAR;}
")"     {return RIGHTPAR;}
"."     {return DOT;}
","     {return COMMA;}

%%