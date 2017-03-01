%option noyywrap
%option nounput

%{
#include "parser/parser.hpp"
#define YY_DECL int yylex()
%}


LABEL [a-zA-Z0-9_]+
DOUBLE [0-9]+(.[0-9]+)?

%%

"arg"     {return ARG;}
"att"     {return ATT;}
"goal"    {return GOAL;}
"frozen"  {return FROZEN;}
"initial" {return INITIAL;}
"words"   {return WORDS;}
"AND"     {return AND;}
"OR"      {return OR;}
"NOT"     {return NOT;}

{LABEL} {yylval.str = strdup(yytext); return LABEL;}

[ \t\n] {}
"("     {return LEFTPAR;}
")"     {return RIGHTPAR;}
"."     {return DOT;}
","     {return COMMA;}
"|"     {return COND;}

%%

void parse(const std::string &line, stardec::parsed_item &item, bool verbose) {
    YY_BUFFER_STATE buffer = yy_scan_string(line.c_str());
    yyparse(item, verbose);
    yy_delete_buffer(buffer);
}
