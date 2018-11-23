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
            TempVarPool &tvp, Operand **ret_ord){
        const string ehd = "Expression: ";
        Tuples tuples;

        // check validness
        if(item_list.size() < 1){
            log::error << "Expression should have at least one item.";
            return tuples;
        }
        if(item_list.size() != op_list.size()+1){
            log::error << "Expression's item and op num are strange!";
            return tuples;
        }

        // evaluate the first item
        if(item_list[0] == NULL)
            throw string("Item should never be NULL.");
        Operand *first_ord = NULL;
        Tuples sub_tuples = item_list[0]->dump(tab, func_name, tvp, &first_ord);
        tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());

        // check whether only one item && not need any operation
        if(item_list.size() == 1 && minus == false){
            *ret_ord = first_ord;
            return tuples;
        }

        throw string("Not implemented");

        // ask for a temp var to save result
        VarEntry *res_int_var = tvp.getNewIntTempVar();
        *ret_ord = new Operand(res_int_var);

        // do minus on the first item
        // first_ord will be replaced
        if(minus){
            // TODO
        }

        // TODO loop process the following items

        return tuples;
    }
};

#endif//EXPRESSION_H