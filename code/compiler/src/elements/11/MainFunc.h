#ifndef MAIN_FUNC_H
#define MAIN_FUNC_H

#include "Element.h"
#include "CompoundStatement.h"

class MainFunc : public Element
{
    public:
        MainFunc();
        virtual ~MainFunc();

        CompoundStatement *compound_statement;
};

#endif // MAIN_FUNC_H
