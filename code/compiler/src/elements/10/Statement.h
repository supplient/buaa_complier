#ifndef STATEMENT_H
#define STATEMENT_H

#include "Element.h"
#include "Expression.h"

class Statement: public Element
{
    public:

};

class IfStatement: public Statement
{

};

class WhileStatement: public Statement
{

};

class FuncCallStatement: public Statement
{

};

class AssignStatement: public Statement
{

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

};

class EmptyStatement: public Statement
{

};

class BracedStatement: public Statement
{

};

#endif//STATEMENT_H