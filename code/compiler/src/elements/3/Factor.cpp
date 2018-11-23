#include "Factor.h"
#include "Integer.h"


Tuples ConstFactor::dump(NameTable &tab, const string &func_name, 
        TempVarPool &tvp, Operand **ret_ord){
    if(is_char)
        *ret_ord = new Operand(char_value);
    else
        *ret_ord = new Operand(int_value->value);
    Tuples tuples;// Just return empty tuples since no executing.
    return tuples;
}