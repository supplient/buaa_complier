#ifndef STATEMENT_H
#define STATEMENT_H

#include "Element.h"

class Expression;
class StatementList;
class FuncCallExp;
class Condition;
class SwitchCaseList;

class Statement: public Element
{
public:
    virtual Tuples dump(NameTable &tab, const string &func_name){
        throw string("Not implemented.");
        Tuples tuples;// Only for no compile warning
        return tuples;
    }
};

class IfStatement: public Statement
{
public:
    Condition *cond;
    Statement *state;
};

class WhileStatement: public Statement
{
public:
    Condition *cond;
    Statement *state;
};

class SwitchStatement: public Statement
{
public:
    Expression *exp;
    SwitchCaseList *case_list;
    Statement *default_state;
};

class FuncCallStatement: public Statement
{
public:
    FuncCallExp *call_exp;
};

class AssignStatement: public Statement
{
public:
    string ident;
    Expression *exp;

    bool is_array;
    Expression *select;
};

class InputStatement: public Statement
{
public:
    vector<string> ident_list;
};

class OutputStatement: public Statement
{
public:
    OutputStatement():Statement(){
        has_string=false;
        }
    bool has_string;
    string str_value;
    Expression *exp_value;
};

class ReturnStatement: public Statement
{
public:
    Expression *exp;
};

class EmptyStatement: public Statement
{
public:
    virtual Tuples dump(NameTable &tab, const string &func_name){
        Tuples tuples;
        // Empty
        return tuples;
    }
};

class BracedStatement: public Statement
{
public:
    StatementList *state_list;

    virtual Tuples dump(NameTable &tab, const string &func_name);
};

#endif//STATEMENT_H