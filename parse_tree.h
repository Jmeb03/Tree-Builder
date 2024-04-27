//****************************************************************
// File: parse_tree.h
// Author: David W. Juedes
//
// Copyright 2007, All rights reserved.
// Purpose: This .h file includes a hierarchy 
// of classes that define possible 
// instructions in the pfig language
//  
// In general, there are statements, 
// boolean expressions, and integer_expressions
//
//******************************************************************

#include <map>
#include <string>
#include <set>
#include <algorithm>
#include <iostream>
#include <memory>
#include <optional>
#include "tree_node.h"

using namespace std;

class integer_expression {
 public:
  virtual int evaluate_expression() =0;
};


class int_constant:public integer_expression {
 public:
  int_constant(int val) {saved_val = val;}

  virtual int evaluate_expression() {
    return saved_val;
  }
 private:
  int saved_val;
};

class string_expression {
public:
    virtual ~string_expression() = default;

    // Evaluate the expression and return the resulting string
    virtual std::string evaluate_expression() const = 0;

    // Display the expression (useful for debugging)
    virtual void display() const = 0;
};

// Derived class for string constants
class string_constant : public string_expression {
private:
    std::string value_;

public:
    explicit string_constant(const std::string& value) : value_(value) {}

    std::string evaluate_expression() const override {
        return value_;
    }

    void display() const override {
        std::cout << "String Constant: \"" << value_ << "\"" << std::endl;
    }
};

// Derived class for variables
class string_variable : public string_expression {
private:
    std::string name_;

public:
    explicit string_variable(const std::string& var_name) : name_(var_name) {}

    std::string evaluate_expression() const override {
        // Placeholder for variable evaluation logic
        return name_; // This would be replaced with actual variable resolution logic
    }

    void display() const override {
        std::cout << "String Variable: " << name_ << std::endl;
    }
};

// Derived class for concatenation expressions
class concat_expression : public string_expression {
private:
    string_expression* left_;
    string_expression* right_;

public:
    concat_expression(string_expression* left, string_expression* right) : left_(left), right_(right) {}

    std::string evaluate_expression() const override {
        return left_->evaluate_expression() + right_->evaluate_expression();
    }

    void display() const override {
        std::cout << "Concatenation: ";
        left_->display();
        std::cout << " + ";
        right_->display();
        std::cout << std::endl;
    }

    ~concat_expression() { // Destructor to manage memory
        delete left_;
        delete right_;
    }
};

class plus_expr: public integer_expression {
 public:
  plus_expr(integer_expression *left, integer_expression *right) {
    l = left;
    r = right;
  }

  virtual int evaluate_expression() {
    return l->evaluate_expression() + r->evaluate_expression();
  }
  
  private:
    integer_expression *l;
    integer_expression *r;
};

class statement {
 public:
  virtual void evaluate_statement(map<string, TreeNode> &sym_tab) =0;
};

class compound_statement: public statement {
 public:
  compound_statement(statement *first, compound_statement *rest) {
    f = first;
    r = rest;
  }
  
  virtual void evaluate_statement(map<string, TreeNode> &sym_tab) {
    if (f!=NULL) {
      f->evaluate_statement(sym_tab);
    }
    if (r!=NULL) {
      r->evaluate_statement(sym_tab);
    }
  }
 private:
  compound_statement *r;
  statement *f;
};
  
class build_statement : public statement {
public:
    // Constructor with a default value for `child_of_expr`
    build_statement(string_expression* name_expr, integer_expression* weight_expr, string_expression* child_of_expr = nullptr)
        : name_expr(name_expr), weight_expr(weight_expr), child_of_expr(child_of_expr) {}

    void evaluate_statement(std::map<std::string, TreeNode>& sym_tab) {
        std::string name = name_expr->evaluate_expression();
        int weight = weight_expr->evaluate_expression();
        std::string parent_name = (child_of_expr) ? child_of_expr->evaluate_expression() : "NONE";

        // Create a new TreeNode
        TreeNode new_node(name, weight);

        sym_tab[name] = new_node;

        if (parent_name != "NONE") { // If it has a parent
            auto parent_iter = sym_tab.find(parent_name);
            if (parent_iter != sym_tab.end()) {
                TreeNode& parent = parent_iter->second;
                parent.addChild(new_node); // Add as a child to the parent
                new_node.setParent(parent); // Establish parent-child relationship
            }
        }

        // Insert the new node into the symbol table
    }

private:
    string_expression* name_expr;
    integer_expression* weight_expr;
    string_expression* child_of_expr;
};


