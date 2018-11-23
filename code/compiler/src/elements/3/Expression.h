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
            TempVarPool &tvp, VarEntry *res_entry){
        Operand *res_ord = new Operand(res_entry);
        return dump(tab, func_name, tvp, &res_ord);
    }

    virtual Tuples dump(NameTable &tab, const string &func_name, 
            TempVarPool &tvp, Operand **res_ord){
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

        // check res_ord
        if(*res_ord){
            // user assign a res_ord to store
            // check whether it is a variable
            if(*res_ord->type != Operand::ENTRY){
                log::error << "Expression received a res_ord of non-entry type.";
                return tuples;
            }
            if(*res_ord->entry->entry_type != sem::VAR_ENTRY){
                log::error << "Expression recevied a res_ord of non-var entry";
                return tuples;
            }

            // It's OK, no operation is needed to be done
        }
        else{
            // ask for a temp var to save result
            VarEntry *temp_var_entry = tvp.getNewIntTempVar();
            *res_ord = new Operand(temp_var_entry);
        }

        // evaluate the first item
        if(item_list[0] == NULL)
            throw string("Item should never be NULL."):
        Tuples sub_tuples = item_list[0]->dump(tab, func_name, tvp, res_ord);
        tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());

        // do minus
        if(minus){
            // TODO
        }

        // TODO loop process the following items

        return tuples;
    }
};

#endif//EXPRESSION_H