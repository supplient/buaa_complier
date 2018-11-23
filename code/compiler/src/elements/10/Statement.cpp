#include "Statement.h"
#include "StatementList.h"
#include "Expression.h"

Tuples AssignStatement::dump_int(NameTable &tab, const string &func_name, TempVarPool &tvp){
    Tuples tuples;

    if(is_array){
        // TODO
        throw string("Not implemented.");
    }
    else{
        // check whether left value exists
        NameTableEntry *left_entry = tab.lookUp(func_name, ident);
        if(!left_entry){
            errorRepo("Variable [" + ident + "] used before declare.");
            return tuples;
        }

        // cast to var entry
        VarEntry *left_var = dynamic_cast<VarEntry*>(left_entry);
        
        // check whether is var
        if(left_var == NULL || left_entry->entry_type != sem::VAR_ENTRY){
            errorRepo("Left_value must be variable.");
            return tuples;
        }

        // check whether is an array name
        if(left_var->dim != 0){
            errorRepo("Cannot assign to an array!");
            return tuples;
        }

        // evaluate right value
        if(!exp){
            log::error << "AssignMent: exp is NULL!";
            exit(-1);
        }
        Operand *right_ord = NULL;
        Tuples sub_tuples = exp->dump(tab, func_name, tvp, &right_ord);
        tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());

        // assign right value to left value
        Operand *left_ord = new Operand(left_var);
        Tuple *assign_tuple = new Tuple();
        assign_tuple->op = sem::ASSIGN;
        assign_tuple->left = right_ord;
        assign_tuple->res = left_ord;
        tuples.insert(tuples.end(), assign_tuple);
    }

    return tuples;
}

Tuples BracedStatement::dump(NameTable &tab, const string &func_name){
    Tuples tuples;

    if(state_list){
        Tuples sub_tuples = state_list->dump(tab, func_name);
        tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());
    }
    else
        log::error << "NULL state_list in BracedStatement";

    return tuples;
}