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
            mylog::error << "Item should have at least one factor.";
            return tuples;
        }
        if(factor_list.size() != op_list.size()+1){
            mylog::error << "Item's factor and op num are strange!";
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

        // ask for a temp var to save result
        VarEntry *res_int_var = tvp.getNewIntTempVar();
        *ret_ord = new Operand(res_int_var);

        // assign the first factor to the temp var
        Tuple *first_assign_tuple = new Tuple();
        first_assign_tuple->op = sem::ASSIGN;
        first_assign_tuple->left = first_ord;
        first_assign_tuple->res = new Operand(res_int_var);
        tuples.push_back(first_assign_tuple);

        first_ord = new Operand(res_int_var);

        // loop process the following
        unsigned int ri = 1;
        Operand *left_ord = first_ord;

        while(ri < factor_list.size()){
            Tuple *tuple = new Tuple();
            switch(op_list[ri-1]){
                case sym::MULTI:
                    tuple->op = sem::MUL;
                    break;
                case sym::DIV:
                    tuple->op = sem::DIV;
                    break;
                default:
                    mylog::error << "Invalid op for item: " << to_string(op_list[0]);
            }
            tuple->left = left_ord;

            // evaluate the next
            Operand *right_ord = NULL;
            Tuples sub_tuples = factor_list[ri]->dump(tab, func_name, tvp, &right_ord);
            tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());
            tuple->right = right_ord;

            // create res ord
            Operand *res_ord = new Operand(res_int_var);
            tuple->res = res_ord;

            tuples.push_back(tuple);

            // change left ord
            left_ord = new Operand(res_int_var);

            ri++;
        }

        return tuples;
    }
};

#endif//ITEM_H
