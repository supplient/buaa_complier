#ifndef COMPOUND_STATEMENT_H
#define COMPOUND_STATEMENT_H

#include "Element.h"
#include "StatementList.h"
#include "ConstDecl.h"
#include "VarDecl.h"

class CompoundStatement : public Element
{
    public:
        CompoundStatement();
        virtual ~CompoundStatement();

        ConstDecl* const_decl;
        VarDecl* var_decl;
        StatementList* statement_list;
};

#endif // COMPOUND_STATEMENT_H
