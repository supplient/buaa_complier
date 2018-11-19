#ifndef FUNC_CALL_EXP_H
#define FUNC_CALL_EXP_H

#include "Element.h"

class Expression;

class FuncCallExp: public Element
{
public:
    string ident;
    vector<Expression*> param_list;
};

#endif// FUNC_CALL_EXP_H