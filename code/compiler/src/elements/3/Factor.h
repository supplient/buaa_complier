#ifndef FACTOR_H
#define FACTOR_H

#include "Element.h"

class Expression;

class Factor: public Element
{

};

class VarFactor: public Factor
{
public:
    bool is_array;
    string ident;
    Expression *select;
};

class FuncFactor: public Factor
{
public:
    // TODO
};

class ExpFactor: public Factor
{
public:
    Expression *exp;
};

class ConstFactor: public Factor
{
public:
    sym::SYMBOL type;
    int int_value;
    char char_value;
};

#endif//FACTOR_H