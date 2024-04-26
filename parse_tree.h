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

using namespace std;

class integer_expression {
 public:
  virtual int evaluate_expression(map<string, int> &sym_tab) =0;
};


class int_constant:public integer_expression {
 public:
  int_constant(int val) {saved_val = val;}

  virtual int evaluate_expression(map<string, int> &sym_tab) {
    return saved_val;
  }
 private:
  int saved_val;
};

class variable: public integer_expression {
 public:
  variable(char *in_val) {//cout << "Found variable = " << in_val << endl; 
                          saved_val =in_val;}

  virtual int evaluate_expression(map<string, int> &sym_tab) {

    map<string,int>::iterator p;
    p =sym_tab.find(saved_val);
    //cout << "Looking up variable " << saved_val << endl;
    if (p!=sym_tab.end()) {
      //cout << "Returning value of variable " << saved_val << endl;
      //cout << "= " << p->second << endl;

      return p->second;
    } else {
      // ERROR .... for now return -1;
      // Should throw error!
      return -1;
    }

  }
 private:
  string saved_val;
  
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

class neg_constant: public integer_expression {
 public:
  neg_constant(integer_expression *ptr) {
    eval_ptr = ptr;
  }
  
  virtual int evaluate_expression(map<string, int> &sym_tab) {
    return -eval_ptr->evaluate_expression(sym_tab);
  }
  integer_expression *eval_ptr;
};



class plus_expr: public integer_expression {
 public:
  plus_expr(integer_expression *left, integer_expression *right) {
    l = left;
    r = right;
  }

  virtual int evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) + r->evaluate_expression(sym_tab);
  }
  
  private:
    integer_expression *l;
    integer_expression *r;
};

class minus_expr: public integer_expression {
 public:
  minus_expr(integer_expression *left, integer_expression *right) {
    l = left;
    r = right;
  }

  virtual int evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) - r->evaluate_expression(sym_tab);
  }
  
  private:
    integer_expression *l;
    integer_expression *r;
};

class mult_expr: public integer_expression {
 public:
  mult_expr(integer_expression *left, integer_expression *right) {
    l = left;
    r = right;
  }

  virtual int evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) * r->evaluate_expression(sym_tab);
  }
  
  private:
    integer_expression *l;
    integer_expression *r;
};


class div_expr: public integer_expression {
 public:
  div_expr(integer_expression *left, integer_expression *right) {
    l = left;
    r = right;
  }

  virtual int evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) / r->evaluate_expression(sym_tab);
  }
  
  private:
    integer_expression *l;
    integer_expression *r;
};

class mod_expr: public integer_expression {
 public:
  mod_expr(integer_expression *left, integer_expression *right) {
    l = left;
    r = right;
  }

  virtual int evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) % r->evaluate_expression(sym_tab);
  }
  
  private:
    integer_expression *l;
    integer_expression *r;
};

class statement {
 public:
  virtual void evaluate_statement(map<string, int> &sym_tab) =0;
};

class compound_statement: public statement {
 public:
  compound_statement(statement *first, compound_statement *rest) {
    f = first;
    r = rest;
  }
  
  virtual void evaluate_statement(map<string, int> &sym_tab) {
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
  



class assignment_statement: public statement {

 public:
  assignment_statement(char *id, integer_expression *rhs) {
    ident = id;
    r_side = rhs;
  }
  virtual void evaluate_statement(map<string, int> &sym_tab) {
    
    int temp = r_side->evaluate_expression(sym_tab);

    //cout << "Assigning" << ident << " to " << temp << endl;

    sym_tab[ident]=temp;
  }


  private: 
    string ident;
    integer_expression *r_side;
  };

class print_statement: public statement {
 public:
  print_statement(integer_expression *expr) {
    e=expr;
  }
  virtual void evaluate_statement(map<string, int> &sym_tab) {
    cout << e->evaluate_expression(sym_tab) << endl;
  }
    

  private:
    integer_expression *e;

};


