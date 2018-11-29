#include "FuncCallExp.h"
#include "Expression.h"

Tuples FuncCallExp::dump(NameTable &tab, const string &func_name,
        TempVarPool &tvp, Operand **ret_ord){
    Tuples tuples;

    NameTableEntry *gen_entry = tab.lookUp(func_name, ident);
    // check whether declared
    if(!gen_entry){
        errorRepo("Identifier " + ident + " is used before declared.");
        return tuples;
    }

    FuncEntry *func_entry = dynamic_cast<FuncEntry*>(gen_entry);
    // check whether is func entry
    if(!func_entry || gen_entry->entry_type != sem::FUNC_ENTRY){
        errorRepo("Identifier " + ident + " is not a func!");
        return tuples;
    }

    // check param list
        // num check
    if(func_entry->param_list.size() != param_list.size()){
        errorRepo("Param number is not equal! Need " + to_string(func_entry->param_list.size()) + ", but " + to_string(param_list.size()) + " is given.");
        return tuples;
    }

    // check whether has return value
    if(ret_ord){
        // caller want a return value
        if(func_entry->return_type == sym::VOID){
            errorRepo("This is a void function, which cannot give a return value.");
            return tuples;
        }
    }

    // dump param list
    for(Expression *exp: param_list){
        if(!exp){
            mylog::error << "FuncCallExp: a param expression is NULL!";
            return tuples;
        }

        Operand *param_ord = NULL;
        Tuples sub_tuples = exp->dump(tab, func_name, tvp, &param_ord);
        tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());

        Tuple *param_tuple = new Tuple();
        param_tuple->op = sem::PARAM;
        param_tuple->res = param_ord;
        tuples.push_back(param_tuple);
    }

    // call function
    Tuple *call_tuple = new Tuple();
    call_tuple->op = sem::CALL;
    call_tuple->res = new Operand(func_entry->name);
    tuples.push_back(call_tuple);

    // dump return operand
    if(ret_ord){
        string rv_name;
        switch(func_entry->return_type){
            case sym::INT:
                rv_name = NameUtil::getIntReturnVarName();
                break;
            case sym::CHAR:
                rv_name = NameUtil::getCharReturnVarName();
                break;
            default:
                mylog::error << "FuncCallExp: Invalid return type: " << func_entry->return_type;
                return tuples;
        }
        *ret_ord = new Operand(tab.lookUp(func_name, rv_name));
    }

    return tuples;
}
