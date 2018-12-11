#include "Factor.h"
#include "Integer.h"
#include "Expression.h"
#include "FuncCallExp.h"

Tuples VarFactor::dump(NameTable &tab, const string &func_name, TempVarPool &tvp, Operand **ret_ord){
    const string ehd = "VarFactor: ";
    Tuples tuples;

    VarEntry *entry = tab.lookUpVar(func_name, ident);

    // check whether declared
    if(!entry){
        errorRepo("Variable " + ident + " is used before being declared.");
        return tuples;
    }

    if(is_array){
        // dim check
        if(entry->dim == 0){
            errorRepo("Variable " + ident + " is not an array, cannot have a selector.");
            return tuples;
        }
        // select check
        if(!select){
            mylog::error << "VarFactor has a NULL select when its is_array is true.";
            return tuples;
        }

        // ask for a temp var to save read result
        // & fill ret_ord
        VarEntry *tv_entry = tvp.getNewTempVar(entry->type);
        *ret_ord = new Operand(tv_entry);

        // construct load tuple
        Tuple *tuple = new Tuple();
        tuple->op = sem::RARRAY;
        tuple->left = new Operand(entry);

        Tuples sel_tuples = select->dump(tab, func_name, tvp, &tuple->right);
        tuples.insert(tuples.end(), sel_tuples.begin(), sel_tuples.end());

        tuple->res = new Operand(tv_entry);

        tuples.push_back(tuple);
    }
    else{
        // dim check
        if(entry->dim != 0){
            errorRepo("Variable " + ident + " is an array, must assign a selector to be a right-value.");
            return tuples;
        }

        *ret_ord = new Operand(entry);
    }

    return tuples;
}

Tuples FuncFactor::dump(NameTable &tab, const string &func_name,
            TempVarPool &tvp, Operand **ret_ord){
    if(!call_exp){
        mylog::error << "FuncFactor: call_exp is NULL.";
        Tuples tuples;
        return tuples;
    }
    return call_exp->dump(tab, func_name, tvp, ret_ord);
}

Tuples ExpFactor::dump(NameTable &tab, const string &func_name,
            TempVarPool &tvp, Operand **ret_ord){
    Tuples tuples;

    if(!exp){
        mylog::error << "ExpFactor has a NULL exp.";
        return tuples;
    }
    tuples = exp->dump(tab, func_name, tvp, ret_ord);

    return tuples;
}

Tuples ConstFactor::dump(NameTable &tab, const string &func_name,
        TempVarPool &tvp, Operand **ret_ord){
    if(is_char)
        *ret_ord = new Operand(char_value);
    else
        *ret_ord = new Operand(int_value->value);
    Tuples tuples;// Just return empty tuples since no executing.
    return tuples;
}
