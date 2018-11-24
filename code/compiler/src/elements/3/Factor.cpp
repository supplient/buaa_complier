#include "Factor.h"
#include "Integer.h"
#include "Expression.h"

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
        // TODO
        throw string("VarFactor::array not implemented.");
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

Tuples ExpFactor::dump(NameTable &tab, const string &func_name,
            TempVarPool &tvp, Operand **ret_ord){
    Tuples tuples;

    if(!exp){
        log::error << "ExpFactor has a NULL exp.";
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
