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
};

#endif// SWITCH_CASE_H