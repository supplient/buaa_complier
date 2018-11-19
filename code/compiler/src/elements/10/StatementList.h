#ifndef STATEMENT_LIST_H
#define STATEMENT_LIST_H

#include "Element.h"
#include "Statement.h"

class StatementList: public Element
{
    public:
        vector<Statement*> state_list;
};

#endif//STATEMENT_LIST_H