#ifndef MAIN_FUNC_H
#define MAIN_FUNC_H

#include "Element.h"
#include "CompoundStatement.h"

class MainFunc : public Element
{
    public:
        MainFunc(){}
        virtual ~MainFunc(){}

        CompoundStatement *compound_statement;

        virtual Tuples dump(NameTable &tab){
            Tuples tuples;
            if(!compound_statement){
                mylog::error << "MainFunc: compound_statement is NULL.";
                return tuples;
            }

            // create a tuple as the entrance
            Tuple *start_tuple = new Tuple();
            start_tuple->op = sem::FUNC;
            start_tuple->res = new Operand(sem::MAIN_FUNC_NAME);
            tuples.insert(tuples.begin(), start_tuple); // should be at the head

            // fill tab
            vector<VarEntry *> empty_param_list;
            if(!tab.insertFunc(sem::MAIN_FUNC_NAME, sym::VOID, empty_param_list, start_tuple))
                errorRepo("multiple defination for main function");

            // dump compound_state
            Tuples state_tuples = compound_statement->dump(tab, sem::MAIN_FUNC_NAME);
            tuples.insert(tuples.end(), state_tuples.begin(), state_tuples.end());

            // do return implictly if the last tuple is not RET
            if(tuples.size() < 1 || tuples[tuples.size()-1]->op != sem::RET){
                Tuple *ret_tuple = new Tuple();
                ret_tuple->op = sem::RET;
                tuples.push_back(ret_tuple);
            }

            return tuples;
        }
};

#endif // MAIN_FUNC_H
