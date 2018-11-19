#ifndef CONDITION_H
#define CONDITION_H

#include "Element.h"

class Expression;

class Condition: public Element
{
public:
    bool has_comp;

    Expression *left_exp;
    sym::SYMBOL op;
    Expression *right_exp;
};

#endif// CONDITION_H