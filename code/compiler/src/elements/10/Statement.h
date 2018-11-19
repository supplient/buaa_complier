#ifndef STATEMENT_H
#define STATEMENT_H

#include "Element.h"

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