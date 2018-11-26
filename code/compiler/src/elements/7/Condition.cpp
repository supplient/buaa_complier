#include "Condition.h"
#include "Expression.h"

Tuples Condition::dump(NameTable &tab, const string &func_name, TempVarPool &tvp, Operand **ret_ord){
    Tuples tuples;
    
    // check at least one exp
    if(!left_exp){
        mylog::error << "Condition: left_exp is NULL!";
        return tuples;
    }

    Operand *left_ord = NULL;
    Operand *right_ord = NULL;

    // evaluate left_ord
    Tuples left_tuples = left_exp->dump(tab, func_name, tvp, &left_ord);
    tuples.insert(tuples.end(), left_tuples.begin(), left_tuples.end());

    if(!has_comp){
        // do not need compare
        *ret_ord = left_ord;
        return tuples;
    }
    // need compare
    
    // check right_exp
    if(!right_exp){
        mylog::error << "Condition: right_exp is NULL when has_comp is true";
        return tuples;
    }

    // evalute right_ord
    Tuples right_tuples = right_exp->dump(tab, func_name, tvp, &right_ord);
    tuples.insert(tuples.end(), right_tuples.begin(), right_tuples.end());

    // ask for temp var
    VarEntry *res_int_var = tvp.getNewIntTempVar();
    *ret_ord = new Operand(res_int_var);

    // do compare
    Tuple *comp_tuple = new Tuple();
    switch(op){
        case sym::LESS: comp_tuple->op = sem::LESS; break;
        case sym::LESSOREQUAL: comp_tuple->op = sem::LESSOREQUAL; break;
        case sym::MORE: comp_tuple->op = sem::MORE; break;
        case sym::MOREOREQUAL: comp_tuple->op = sem::MOREOREQUAL; break;
        case sym::NOTEQUAL: comp_tuple->op = sem::NOTEQUAL; break;
        case sym::EQUAL: comp_tuple->op = sem::EQUAL; break;
        default:
            mylog::error << "Condition: invalid operator: " << op;
            return tuples;
    }
    comp_tuple->left = left_ord;
    comp_tuple->right = right_ord;
    comp_tuple->res = new Operand(res_int_var);
    tuples.push_back(comp_tuple);

    return tuples;
}