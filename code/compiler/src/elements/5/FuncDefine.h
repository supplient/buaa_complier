#ifndef FUNC_DEFINE_H
#define FUNC_DEFINE_H

#include "Element.h"

class Param;
class CompoundStatement;

class FuncDefine: public Element
{
public:
    sym::SYMBOL return_type;
    string func_name;
    vector<Param*> param_list;
    CompoundStatement *compound_state;
};

#endif//FUNC_DEFINE_H