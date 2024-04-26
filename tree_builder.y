%{
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include "parse_tree.h"

using namespace std;
%}

%union {
    integer_expression* i_ptr;
    string_expression* str_ptr;
    char* sval; // For storing strings
    statement* s_ptr;
    compound_statement* c_ptr;
}

%token <sval> TKID TKSTRING TKINT
%token TKBUILD TKFOR TKCHILD TKWEIGHT TKNAME 

%type <str_ptr> string_expression 
%type <i_ptr> integer_expression
%type <s_ptr> statement build_statement
%type <c_ptr> prog start_var


%{
extern int yylex();
extern void yyerror(char *String);  

#include <iostream>
 using namespace std;

%}

%%


start_var : prog { // At this point, the 
                   // the program is done --- let's evaluate the
                   // program
                   map<string,int> my_sym_tab;
                   $$= $1;
                   $1->evaluate_statement(my_sym_tab);
}


prog: statement prog {$$ = new compound_statement($1,$2);}
    | {$$ = NULL;}
    ;

statement: build_statement {$$ = $1;}
         ;

build_statement: TKBUILD '{' TKNAME '=' string_expression ';' TKWEIGHT'=' integer_expression ';' TKCHILD '=' string_expression ';' '}' ';' {
    cout << "Name: " << $5 << " Weight: " << $9 << " Parent: " << $13 << endl;
    $$ = NULL;}	
    | TKBUILD '{' TKNAME '=' string_expression ';' TKWEIGHT '=' integer_expression ';' '}' ';' {
    cout << "Name: " << $5 << " Weight: " << $9 << " Parent: NONE" << endl;
    $$ = NULL;}
    ;

integer_expression: TKINT {cout << "Integer: " << $1 << endl;
		      $$ = new int_constant(atoi($1));}
| TKID {cout << "Identifier: " << $1 << endl; 
		        $$= new variable($1); }
                | integer_expression '+' integer_expression {$$=new plus_expr($1,$3);}
                ;

string_expression: TKSTRING {
    cout << "String: " << $1 << endl;
    if ($1 != nullptr) {
        $$ = new string_constant($1); // Ensure it's not null
    } else {
        yyerror("Null string encountered (TKSTRING)");
    }
}
| TKID {
    if ($1 != nullptr) {
        $$ = new string_variable($1); // Ensure valid initialization
    } else {
        yyerror("Null identifier encountered (TKID)");
    }
}
| string_expression '+' string_expression {
    $$ = new concat_expression($1, $3); // Ensure proper cleanup with raw pointers
}
| string_expression '+' integer_expression {
    std::map<std::string, int> temp_map; // Declare the map variable
    std::string int_str = std::to_string($3->evaluate_expression(temp_map)); // Convert to string
    $$ = new concat_expression($1, new string_constant(int_str)); // Use the variable
}



%%
#include "lex.yy.c"

void yyerror(char *error_string){
  cout << "Error : " << error_string << "  on line " << line_nmb() << endl;
  exit(-1);
}

int main() {
    return yyparse();
}