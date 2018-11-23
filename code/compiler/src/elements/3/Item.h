#ifndef ITEM_H
#define ITEM_H

#include "Element.h"
#include "Factor.h"

class Item: public Element
{
public:
    vector<Factor*> factor_list;
    vector<sym::SYMBOL> op_list;

    virtual Tuples dump(NameTable &tab, const string &func_name, 
            TempVarPool &tvp, Operand **res_ord){
        const string ehd = "Item: ";
        Tuples tuples;

        if(factor_list.size() < 1){
            log::error << "Item should have at least one factor.";
            return tuples;
        }
        if(factor_list.size() != op_list.size()+1){
            log::error << "Item's factor and op num are strange.";
            return tuples;
        }

        if(*res_entry == NULL){
            log::error << "Item received a res_ord of NULL.";
            return tuples;
        }

        if(factor_list[0] == NULL)
            throw string("Factor should never be NULL.");
        Tuples sub_tuples = factor_list[0]->dump(tab, func_name, tvp, res_ord);
        tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());

        // TODO loop process the following factors

        return tuples;
    }
};

#endif//ITEM_H