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

#define TKBUILDNODE 1 
#define TKFOR 2
#define TKID 3 
#define TKNAME 4
#define TKNUM 5
#define TKSTRING 6
#define TKWEIGHT 7
#define TKCHILD 8

%}

%option noyywrap 

%%

"buildnode"  { return TKBUILDNODE; }
"for"        { return TKFOR; }
"name"       {  return TKNAME;  }
"weight"     {  return TKWEIGHT; }
"isachildof" {  return TKCHILD; }
\"(\\.|[^\"\\])*\"   { return TKSTRING; }
[0-9]+     { return TKNUM; }
\b[a-zA-Z_][a-zA-Z0-9_]*\b { return TKID; }

[ \t\n]+    { /* ignore whitespace */ }
.           { printf("Unexpected character: %c\n", yytext[0]); }
%%


int main(){
int token;
    
    // Call yylex() to initialize the lexer and get the first token
    while ((token = yylex()) != 0) {
        switch (token) {
            case TKBUILDNODE:
                std::cout << "Token: TKBUILDNODE" << std::endl;
                break;
            case TKFOR:
                std::cout << "Token: TKFOR" << std::endl;
                break;
            case TKNAME:
                std::cout << "Token: TKNAME" << std::endl;
                break;
            case TKID:
                std::cout << "Token: TKID (" << yytext << ")" << std::endl;
                break;
            case TKNUM:
                std::cout << "Token: TKNUM (" << yytext << ")" << std::endl;
                break;
            case TKSTRING:
                std::cout << "Token: TKSTRING (" << yytext << ")" << std::endl;
                break;
            case TKWEIGHT:
                std::cout << "Token: TKWEIGHT (" << yytext << ")" << std::endl;
                break;
            case TKCHILD:
                std::cout << "Token: TKCHILD (" << yytext << ")" << std::endl;
                break;
            default:
                std::cout << "Unknown token: " << yytext << std::endl;
                break;
        }
    }

    return 0;
}