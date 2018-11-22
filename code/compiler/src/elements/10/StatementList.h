#ifndef STATEMENT_LIST_H
#define STATEMENT_LIST_H

#include "Element.h"
#include "Statement.h"

class StatementList: public Element
{
public:
    vector<Statement*> state_list;

    virtual Tuples dump(NameTable &tab, const string &func_name){
        Tuples tuples;

        for(Statement *state: state_list){
            if(!state)
                throw string("NULL statement!");
            Tuples sub_tuples = state->dump(tab, func_name);
            tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());
        }

        return tuples;
    }
};

#endif//STATEMENT_LIST_H