%{
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include "parse_tree.h"

using namespace std;
%}

%union {
    int ival;  // For storing integers
    char* sval; // For storing strings
}

%token <ival> NUMBER
%token <sval> IDENTIFIER
%token BUILDNODE FOR EQUAL PLUS COLON OPEN_BRACE CLOSE_BRACE SEMI_COLON

%{
extern int yylex();
extern void yyerror(char *String);  

#include <iostream>
 using namespace std;

%}

%%

program:
    statement_list
    ;

statement_list:
    statement_list statement
    | /* empty */
    ;

statement:
    BUILDNODE OPEN_BRACE parameter_list CLOSE_BRACE
    | FOR IDENTIFIER EQUAL IDENTIFIER PLUS NUMBER COLON NUMBER CLOSE_BRACE
    ;

parameter_list:
    parameter_list parameter
    | parameter
    ;

parameter:
    IDENTIFIER EQUAL NUMBER
    | IDENTIFIER EQUAL IDENTIFIER
    ;

buildnode: 
    TKBUILD "{" TKNAME "=" string_expression ";" TKWEIGHT "=" integer_expression ";" TKISACHILDOF "=" string_expression ";" "}" ";"	{$$ = new build_statement(my_sym_tab, $5, $9, $13);}


%%
#include "lex.yy.c"

void yyerror(char *error_string){
  cout << "Error : " << error_string << "  on line " << line_nmb() << endl;
  exit(-1);
}

int main() {
    return yyparse();
}