#ifndef FUNC_CALL_EXP_H
#define FUNC_CALL_EXP_H

#include "Element.h"

class Expression;

class FuncCallExp: public Element
{
public:
    string ident;
    vector<Expression*> param_list;

    virtual Tuples dump(NameTable &tab, const string &func_name, 
            TempVarPool &tvp, Operand **ret_ord=NULL); // function call expecting return value
};

#endif// FUNC_CALL_EXP_H