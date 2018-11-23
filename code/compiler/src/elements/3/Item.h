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
            TempVarPool &tvp, Operand **ret_ord){
        const string ehd = "Item: ";
        Tuples tuples;

        // check validness
        if(factor_list.size() < 1){
            log::error << "Item should have at least one factor.";
            return tuples;
        }
        if(factor_list.size() != op_list.size()+1){
            log::error << "Item's factor and op num are strange!";
            return tuples;
        }

        // evaluate the first factor
        if(factor_list[0] == NULL)
            throw string("Factor should never be NULL.");
        Operand *first_ord = NULL;
        Tuples sub_tuples = factor_list[0]->dump(tab, func_name, tvp, &first_ord);
        tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());

        // check whether only one factor
        if(factor_list.size() == 1){
            *ret_ord = first_ord;
            return tuples;
        }

        throw string("Not implemented");

        // ask for a temp var to save result
        VarEntry *res_int_var = tvp.getNewIntTempVar();
        *ret_ord = new Operand(res_int_var);

        // TODO loop process the following items

        return tuples;
    }
};

#endif//ITEM_H