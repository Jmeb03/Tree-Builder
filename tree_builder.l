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

alpha       [a-zA-Z]
digit       [0-9]
alphanum    [a-zA-Z0-9]
identifier  {alpha}({alphanum})*
number      {digit}+
quoted_str  \"[^\"]*\"

%%

"buildnode"  { return BUILDNODE; }
"for"        { return FOR; }
"="          { return EQUAL; }
"+"          { return PLUS; }
":"          { return COLON; }
"{"          { return OPEN_BRACE; }
"}"          { return CLOSE_BRACE; }
";"          { return SEMI_COLON; }
{quoted_str}  {yylval.sval = strdup(yytext); return IDENTIFIER; }
{identifier} { yylval.sval = strdup(yytext); return IDENTIFIER; }
{number}     { yylval.ival = atoi(yytext); return NUMBER; }

[ \t\n]+    { /* ignore whitespace */ }
.           { printf("Unexpected character: %c\n", yytext[0]); }

%%
