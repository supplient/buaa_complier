#ifndef SWITCH_CASE_H
#define SWITCH_CASE_H

#include "Element.h"

class Integer;
class Statement;

class SwitchCase: public Element
{
public:
    bool is_int_const;
    Integer *int_value;
    char char_value;
    Statement *state;

    virtual Tuples dump(NameTable &tab, const string &func_name, TempVarPool &tvp, Operand **ret_ord);

    bool operator==(const SwitchCase &b);
};

#endif// SWITCH_CASE_H