#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "Element.h"
#include "Item.h"

class Expression: public Element
{
public:
    Expression():Element(){
        minus = false;
    }
    bool minus;
    vector<Item*> item_list;
    vector<sym::SYMBOL> op_list;
};

#endif//EXPRESSION_H