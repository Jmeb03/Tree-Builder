%{
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include "parse_tree.h"

std::map<std::string, TreeNode>* parser_result = nullptr;

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
%start start_var



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
                   map<string,TreeNode> my_sym_tab;
                   $$= $1;
                    
                    if($$ != nullptr){
                        $1->evaluate_statement(my_sym_tab); // Populate the symbol table
                        parser_result = new std::map<std::string, TreeNode>(my_sym_tab);        
                    }else{
                        $$ = nullptr;
                    }
}


prog: statement prog {$$ = new compound_statement($1,$2);}
    | {$$ = NULL;}
    ;

statement: build_statement {$$ = $1;}
         ;

build_statement: TKBUILD '{' TKNAME '=' string_expression ';' TKWEIGHT'=' integer_expression ';' TKCHILD '=' string_expression ';' '}' ';' {
    $$ = new build_statement($5, $9, $13);}	
    | TKBUILD '{' TKNAME '=' string_expression ';' TKWEIGHT '=' integer_expression ';' '}' ';' {
    $$ = new build_statement($5, $9);}
    ;

integer_expression: TKINT {
		      $$ = new int_constant(atoi($1));}
            | integer_expression '+' integer_expression {$$=new plus_expr($1,$3);}
                ;

string_expression: TKSTRING {
    if ($1 != nullptr) {
        $$ = new string_constant($1); // Ensure it's not null
    } else {
        yyerror("Null string encountered (TKSTRING)");
    }
}
| string_expression '+' string_expression {
    $$ = new concat_expression($1, $3); // Ensure proper cleanup with raw pointers
}
| string_expression '+' integer_expression {
    std::map<std::string, int> temp_map; // Declare the map variable
    std::string int_str = std::to_string($3->evaluate_expression()); // Convert to string
    $$ = new concat_expression($1, new string_constant(int_str)); // Use the variable
}



%%
#include "lex.yy.c"

void yyerror(char *error_string){
  cout << "Error : " << error_string << "  on line " << line_nmb() << endl;
  exit(-1);
}

int main() {
    if (yyparse() == 0) {
       cout << "Symbol table:" << endl;
       string root_str = "\"root\"";
        for (const auto& entry : *parser_result) {
            //std::cout << "Key: [" << entry.first << "], Node: " << entry.second.getName() << std::endl;
            if(entry.first == root_str){
                entry.second.display();
                cout << endl;
            }
        }
    } else {
        std::cerr << "Parsing failed." << std::endl;
    }

    return 0;
}
