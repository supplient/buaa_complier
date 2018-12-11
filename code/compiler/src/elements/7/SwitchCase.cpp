#include "SwitchCase.h"
#include "Integer.h"
#include "Statement.h"

Tuples SwitchCase::dump(NameTable &tab, const string &func_name, TempVarPool &tvp, Operand **ret_ord){
    Tuples tuples;

    // check
    if(!state){
        mylog::error << "SwitchCase: state is NULL.";
        return tuples;
    }
    if(is_int_const && !int_value){
        mylog::error << "SwitchCase: int_value is NULL when is_int_const is true.";
        return tuples;
    }

    // fill ret_ord
    if(is_int_const)
        *ret_ord = new Operand(int_value->value);
    else
        *ret_ord = new Operand(char_value);

    return state->dump(tab, func_name);
}

bool SwitchCase::operator==(const SwitchCase &b){
    // Note: Only check whether case constant equals
    if(is_int_const != b.is_int_const)
        return false;

    if(is_int_const)
        return int_value->value == b.int_value->value;
    else
        return char_value == b.char_value;
}
