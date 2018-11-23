#ifndef FACTOR_H
#define FACTOR_H

#include "Element.h"
#include "FuncCallExp.h"

class Expression;
class Integer;

class Factor: public Element
{
public:
    virtual Tuples dump(NameTable &tab, const string &func_name, 
            TempVarPool &tvp, VarEntry *res_entry=NULL){
        throw string("Not implemented.");
        Tuples tuples;
        return tuples;
    }
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

    virtual Tuples dump(NameTable &tab, const string &func_name, 
            TempVarPool &tvp, VarEntry *res_entry=NULL){
        if(is_char)// TODO char need conv into int
            throw string("Not implemented.");
        Tuples tuples;// Just return empty tuples since no executing.
        return tuples;
    }
};

#endif//FACTOR_H