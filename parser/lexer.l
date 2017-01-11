%option noyywrap
%option nounput

%{
#include "parser.hpp"
#define YY_DECL extern "C" int yylex()
%}

LABEL [a-zA-Z0-9_]+
DOUBLE [0-9]+(.[0-9]+)?

%%

"arg"     {return ARG;}
"att"     {return ATT;}
"goal"    {return GOAL;}
"flock"   {return FLOCK;}
"frozen"  {return FROZEN;}
"initial" {return INITIAL;}
"words"   {return WORDS;}
"AND"     {return AND;}
"OR"      {return OR;}
"NOT"     {return NOT;}

{LABEL} {yylval.str = strdup(yytext); return LABEL;}
{DOUBLE} {yylval.dbl = std::stod(yytext); return DBL;}

[ \t\n] {}
"("     {return LEFTPAR;}
")"     {return RIGHTPAR;}
"."     {return DOT;}
","     {return COMMA;}
"|"     {return COND;}

%%
