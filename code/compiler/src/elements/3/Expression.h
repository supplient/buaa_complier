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

        // ask for a temp var to save result
        VarEntry *res_int_var = tvp.getNewIntTempVar();
        *ret_ord = new Operand(res_int_var);

        // do minus on the first item
        // first_ord will be replaced
        if(minus){
            Operand *neg_ord = new Operand(res_int_var);
            Tuple *neg_tuple = new Tuple();
            neg_tuple->op = sem::NEG;
            neg_tuple->left = first_ord;
            neg_tuple->res = neg_ord;
            tuples.push_back(neg_tuple);

            first_ord = new Operand(res_int_var);
        }

        // loop process the following
        unsigned int ri = 1;
        Operand *left_ord = first_ord;

        do{
            Tuple *tuple = new Tuple();
            switch(op_list[ri-1]){
                case sym::PLUS:
                    tuple->op = sem::ADD;
                    break;
                case sym::MINUS:
                    tuple->op = sem::SUB;
                    break;
                default:
                    log::error << "Invalid op for expression: " << to_string(op_list[0]);
            }
            tuple->left = left_ord;

            // evaluate the next
            Operand *right_ord = NULL;
            Tuples sub_tuples = item_list[ri]->dump(tab, func_name, tvp, &right_ord);
            tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());
            tuple->right = right_ord;

            // create res ord
            Operand *res_ord = new Operand(res_int_var);
            tuple->res = res_ord;

            tuples.push_back(tuple);

            // change left ord
            left_ord = new Operand(res_int_var);

            ri++;
        }while(ri < item_list.size());

        return tuples;
    }
};

#endif//EXPRESSION_H