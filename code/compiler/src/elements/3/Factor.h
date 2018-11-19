#ifndef FACTOR_H
#define FACTOR_H

#include "Element.h"
#include "FuncCallExp.h"

class Expression;
class Integer;

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
    FuncCallExp *call_exp;
};

class ExpFactor: public Factor
{
public:
    Expression *exp;
};

class ConstFactor: public Factor
{
public:
    bool is_char;
    Integer *int_value;
    char char_value;
};

#endif//FACTOR_H