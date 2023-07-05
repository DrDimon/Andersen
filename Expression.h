#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

#include "types.h"

class ObjectInstance;

class Expression {
  public:
    virtual int eval_int(inst_subobjects_map namedObjects) {return eval_bool(namedObjects) ? 1 : 0;};
    virtual bool eval_bool(inst_subobjects_map namedObjects) {return eval_int(namedObjects) != 0;};
    virtual void execute(inst_subobjects_map) {};
    virtual std::string to_string(inst_subobjects_map) {return "error";};
};

class Int : public Expression {
  int i;

  public:
    Int(int i) {this->i = i;};
    int eval_int(inst_subobjects_map) override {return i;};
    std::string to_string(inst_subobjects_map) override {return std::to_string(i);};
};

class Assign : public Expression {
  std::string objname;
  std::string varname;
  Expression *expression;
  public:
    Assign(std::string objname, std::string varname, Expression *expression) {this->objname = objname; this->varname = varname; this->expression = expression;};
    void execute(inst_subobjects_map namedObjects) override;
    bool eval_bool(inst_subobjects_map) override {return true;};
    std::string to_string(inst_subobjects_map namedObjects) override {return objname + "." + varname + expression->to_string(namedObjects);};
};

class Variable : public Expression {
  std::string objname;
  std::string varname;
  public:
    Variable(std::string objname, std::string varname) {this->objname = objname; this->varname = varname;};
    int eval_int(inst_subobjects_map namedObjects) override;
    std::string to_string(inst_subobjects_map namedObjects) override { return "[" + objname + "." + varname + "=" + std::to_string(eval_int(namedObjects)) + "]";};

};

class Add : public Expression {
  Expression *left;
  Expression *right;
  public:
    Add(Expression *left, Expression *right) {this->left = left; this->right = right;};
    int eval_int(inst_subobjects_map namedObjects) override {return left->eval_int(namedObjects) + right->eval_int(namedObjects);};
    std::string to_string(inst_subobjects_map namedObjects) override {
      return left->to_string(namedObjects) + "+" + right->to_string(namedObjects);
    };
};

class Sub : public Expression {
  Expression *left;
  Expression *right;
  public:
    Sub(Expression *left, Expression *right) {this->left = left; this->right = right;};
    int eval_int(inst_subobjects_map namedObjects) override {return left->eval_int(namedObjects) - right->eval_int(namedObjects);};
    std::string to_string(inst_subobjects_map namedObjects) override {
      return left->to_string(namedObjects) + "-" + right->to_string(namedObjects);
    };
};

class Div : public Expression {
  Expression *left;
  Expression *right;
  public:
    Div(Expression *left, Expression *right) {this->left = left; this->right = right;};
    int eval_int(inst_subobjects_map namedObjects) override {return left->eval_int(namedObjects) / right->eval_int(namedObjects);};
    std::string to_string(inst_subobjects_map namedObjects) override {
      return left->to_string(namedObjects) + "/" + right->to_string(namedObjects);
    };
};

class Mult : public Expression {
  Expression *left;
  Expression *right;
  public:
    Mult(Expression *left, Expression *right) {this->left = left; this->right = right;};
    int eval_int(inst_subobjects_map namedObjects) override {return left->eval_int(namedObjects) * right->eval_int(namedObjects);};
    std::string to_string(inst_subobjects_map namedObjects) override {
      return left->to_string(namedObjects) + "*" + right->to_string(namedObjects);
    };
};

class Eq : public Expression {
  Expression *left;
  Expression *right;
  public:
    Eq(Expression *left, Expression *right) {this->left = left; this->right = right;};
    bool eval_bool(inst_subobjects_map namedObjects) override {return left->eval_int(namedObjects) == right->eval_int(namedObjects);};
    std::string to_string(inst_subobjects_map namedObjects) override {
      return left->to_string(namedObjects) + "==" + right->to_string(namedObjects);
    };
};

class Le : public Expression {
  Expression *left;
  Expression *right;
  public:
    Le(Expression *left, Expression *right) {this->left = left; this->right = right;};
    bool eval_bool(inst_subobjects_map namedObjects) override {return left->eval_int(namedObjects) <= right->eval_int(namedObjects);};
    std::string to_string(inst_subobjects_map namedObjects) override {
      return left->to_string(namedObjects) + "<=" + right->to_string(namedObjects);
    };
};

class Ge : public Expression {
  Expression *left;
  Expression *right;
  public:
    Ge(Expression *left, Expression *right) {this->left = left; this->right = right;};
    bool eval_bool(inst_subobjects_map namedObjects) override {return left->eval_int(namedObjects) >= right->eval_int(namedObjects);};
    std::string to_string(inst_subobjects_map namedObjects) override {
      return left->to_string(namedObjects) + ">=" + right->to_string(namedObjects);
    };
};

class Lt : public Expression {
  Expression *left;
  Expression *right;
  public:
    Lt(Expression *left, Expression *right) {this->left = left; this->right = right;};
    bool eval_bool(inst_subobjects_map namedObjects) override {return left->eval_int(namedObjects) < right->eval_int(namedObjects);};
    std::string to_string(inst_subobjects_map namedObjects) override {
      return left->to_string(namedObjects) + "<" + right->to_string(namedObjects);
    };
};

class Gt : public Expression {
  Expression *left;
  Expression *right;
  public:
    Gt(Expression *left, Expression *right) {this->left = left; this->right = right;};
    bool eval_bool(inst_subobjects_map namedObjects) override {return left->eval_int(namedObjects) > right->eval_int(namedObjects);};
    std::string to_string(inst_subobjects_map namedObjects) override {
      return left->to_string(namedObjects) + ">" + right->to_string(namedObjects);
    };
};

class Ne : public Expression {
  Expression *left;
  Expression *right;
  public:
    Ne(Expression *left, Expression *right) {this->left = left; this->right = right;};
    bool eval_bool(inst_subobjects_map namedObjects) override {return left->eval_int(namedObjects) != right->eval_int(namedObjects);};
    std::string to_string(inst_subobjects_map namedObjects) override {
      return left->to_string(namedObjects) + "!=" + right->to_string(namedObjects);
    };
};

class And : public Expression {
  Expression *left;
  Expression *right;
  public:
    And(Expression *left, Expression *right) {this->left = left; this->right = right;};
    bool eval_bool(inst_subobjects_map namedObjects) override {return left->eval_bool(namedObjects) && right->eval_bool(namedObjects);};
    std::string to_string(inst_subobjects_map namedObjects) override {
      return left->to_string(namedObjects) + "&&" + right->to_string(namedObjects);
    };
};

class Or : public Expression {
  Expression *left;
  Expression *right;
  public:
    Or(Expression *left, Expression *right) {this->left = left; this->right = right;};
    bool eval_bool(inst_subobjects_map namedObjects) override {return left->eval_bool(namedObjects) ||  right->eval_bool(namedObjects);};
    std::string to_string(inst_subobjects_map namedObjects) override {
      return left->to_string(namedObjects) + "||" + right->to_string(namedObjects);
    };
};

class Func : public Expression {
  std::string functionName;
  public:
    int eval_int(inst_subobjects_map namedObjects) override;
    std::string to_string(inst_subobjects_map namedObjects) override { return "FUNC." + functionName; }
    Func(const std::string& functionName) : functionName(functionName) {}

};

#endif
