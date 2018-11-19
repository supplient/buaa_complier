#ifndef ITEM_H
#define ITEM_H

#include "Element.h"
#include "Factor.h"

class Item: public Element
{
public:
    vector<Factor*> factor_list;
    vector<sym::SYMBOL> op_list;
};

#endif//ITEM_H