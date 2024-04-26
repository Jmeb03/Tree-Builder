%{
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <cstdlib>
using namespace std;

int linecount=1;
int indent;
int n;

void ACC(char c)
{
  if (c == '\n')
    {
      linecount++;
    }
}
int line_nmb()
{
  return linecount;
}



%}

%option noyywrap 

%%

"buildnode"  { return TKBUILD; }
"for"        { return TKFOR; }
"name"       {  return TKNAME;  }
"weight"     {  return TKWEIGHT; }
"isachildof" {  return TKCHILD; }
\"(\\.|[^\"\\])*\"   { yylval.sval = strdup(yytext); return TKSTRING; }
'([^\\']|\\.)*'     { yylval.sval = strdup(yytext); return TKSTRING; }
[0-9]+     { yylval.sval = strdup(yytext); return TKINT; }
\b[a-zA-Z_][a-zA-Z0-9_]*\b { return TKID; }

[ \t\n]+    { /* ignore whitespace */ }
.           { return yytext[0]; }
%%
