#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "Element.h"
#include "Item.h"

class Expression: public Element
{
public:
    Expression():Element(){
        minus = false;
    }
    bool minus;
    vector<Item*> item_list;
    vector<sym::SYMBOL> op_list;

    virtual Tuples dump(NameTable &tab, const string &func_name, 
            TempVarPool &tvp, VarEntry *res_entry=NULL){
        const string ehd = "Expression: ";
        Tuples tuples;

        if(!res_entry)
            res_entry = tvp.getNewIntTempVar();
        // TODO

        return tuples;
    }
};

#endif//EXPRESSION_H