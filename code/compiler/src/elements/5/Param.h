#ifndef PARAM_H
#define PARAM_H

#include "Element.h"

class Param: public Element
{
public:
    Param(){}
    virtual ~Param(){}

    sym::SYMBOL type;
    string ident;

    virtual VarEntry* dump(NameTable &tab, string func_name, unsigned int index){
        VarEntry *entry = tab.insertParam(func_name, ident, type, index);
        if(entry == NULL)
            errorRepo("multi defination for param: " + ident);
        return entry;
    }
};

#endif//PARAM_H