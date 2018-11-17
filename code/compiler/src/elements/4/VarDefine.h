#ifndef VAR_DEFINE_H
#define VAR_DEFINE_H

#include "Element.h"
#include "symbol.h"

class VarDefine: public Element
{
    public:
        sym::SYMBOL type;
        vector<string> ident_list;
        vector<unsigned int> dim_list;
};

#endif//VAR_DEFINE_H
