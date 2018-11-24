#include "Statement.h"
#include "StatementList.h"
#include "Expression.h"
#include "FuncCallExp.h"

Tuples FuncCallStatement::dump_int(NameTable &tab, const string &func_name, TempVarPool &tvp){
    if(!call_exp){
        log::error << "FunCallStatement: call_exp is NULL";
        Tuples tuples;
        return tuples;
    }
    return call_exp->dump(tab, func_name, tvp);
}

Tuples AssignStatement::dump_int(NameTable &tab, const string &func_name, TempVarPool &tvp){
    Tuples tuples;

    // check whether left value exists
    NameTableEntry *left_entry = tab.lookUp(func_name, ident);
    if(!left_entry){
        errorRepo("Identifier [" + ident + "] used before declare.");
        return tuples;
    }

    // cast to var entry
    VarEntry *left_var = dynamic_cast<VarEntry*>(left_entry);

    // check whether is var
    if(left_var == NULL || left_entry->entry_type != sem::VAR_ENTRY){
        errorRepo("Left_value must be variable.");
        return tuples;
    }

    // check whether is const var
    if(left_var->is_const){
        errorRepo("Left value should not be const.");
        return tuples;
    }

    // check right value
    if(!exp){
        log::error << "AssignMent: exp is NULL!";
        exit(-1);
    }

    // evaluate right value
    Operand *right_ord = NULL;
    Tuples sub_tuples = exp->dump(tab, func_name, tvp, &right_ord);
    tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());

    if(is_array){
        // check dim
        if(left_var->dim == 0){
            errorRepo("Cannot assign a selector for a non-array var.");
            return tuples;
        }

        // evaluate selector
        Operand *sel_ord = NULL;
        Tuples sel_tuples = select->dump(tab, func_name, tvp, &sel_ord);
        tuples.insert(tuples.end(), sel_tuples.begin(), sel_tuples.end());

        // assign
        Tuple *assign_tuple = new Tuple();
        assign_tuple->op = sem::WARRAY;
        assign_tuple->left = sel_ord;
        assign_tuple->right = right_ord;
        assign_tuple->res = new Operand(left_var);
        tuples.push_back(assign_tuple);
    }
    else{

        // check whether is an array name
        if(left_var->dim != 0){
            errorRepo("Cannot assign to an array!");
            return tuples;
        }

        // assign right value to left value
        Tuple *assign_tuple = new Tuple();
        assign_tuple->op = sem::ASSIGN;
        assign_tuple->left = right_ord;
        assign_tuple->res = new Operand(left_var);
        tuples.push_back(assign_tuple);
    }

    return tuples;
}

Tuples ReturnStatement::dump_int(NameTable &tab, const string &func_name, TempVarPool &tvp){
    Tuples tuples;

    // check function's return type
    FuncEntry *func_entry = tab.lookUpFunc(func_name);
    if(func_entry->return_type == sym::VOID && exp){
        errorRepo("Function " + func_name + " should have no return value, while a return expression is given.");
        return tuples;
    }
    if(func_entry->return_type != sym::VOID && !exp){
        errorRepo("Function " + func_name + " must have a return value.");
        return tuples;
    }

    if(func_entry->return_type != sym::VOID){
        // function has return value

        // evaluate return value
        Operand *rv_ord = NULL;
        Tuples rv_tuples = exp->dump(tab, func_name, tvp, &rv_ord);
        tuples.insert(tuples.end(), rv_tuples.begin(), rv_tuples.end());

        // assign to return var
        string rv_name;
        switch(func_entry->return_type){
            case sym::INT:
                rv_name = NameUtil::getIntReturnVarName();
                break;
            case sym::CHAR:
                rv_name = NameUtil::getCharReturnVarName();
                break;
            default:
                log::error << "Invalid type for return value: " << func_entry->return_type;
                return tuples;
        }
        Tuple *assign_tuple = new Tuple();
        assign_tuple->op = sem::ASSIGN;
        assign_tuple->res = new Operand(tab.lookUp(func_name, rv_name));
        assign_tuple->left = rv_ord;
        tuples.push_back(assign_tuple);
    }

    // return
    Tuple *return_tuple = new Tuple();
    return_tuple->op = sem::RET;
    tuples.push_back(return_tuple);

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
