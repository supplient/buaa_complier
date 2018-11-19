#ifndef PARAM_H
#define PARAM_H

#include "Element.h"

class Param: public Element
{
public:
    sym::SYMBOL type;
    string ident;
};

#endif//PARAM_H