#ifndef COMPOUND_STATEMENT_H
#define COMPOUND_STATEMENT_H

#include "Element.h"
#include "StatementList.h"
#include "ConstDecl.h"
#include "VarDecl.h"

class CompoundStatement : public Element
{
    public:
        CompoundStatement(){}
        virtual ~CompoundStatement(){}

        ConstDecl* const_decl;
        VarDecl* var_decl;
        StatementList* statement_list;

    virtual Tuples dump(NameTable &tab, const string &func_name){
        Tuples tuples;

        if(const_decl){
            Tuples sub_tuples = const_decl->dump(tab, func_name);
            tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());
        }

        if(var_decl){
            Tuples sub_tuples = var_decl->dump(tab, func_name);
            tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());
        }

        Tuples state_tuples = statement_list->dump(tab, func_name);
        tuples.insert(tuples.end(), state_tuples.begin(), state_tuples.end());

        return tuples;
    }
};

#endif // COMPOUND_STATEMENT_H
