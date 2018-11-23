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

    virtual VarEntry* dump(NameTable &tab, string func_name){
        VarEntry *entry = tab.insertVar(func_name, ident, type, 0); // We do not allow array param
        if(entry == NULL)
            errorRepo("multi defination for param: " + ident);
        return entry;
    }
};

#endif//PARAM_H