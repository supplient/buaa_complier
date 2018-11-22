#ifndef FUNC_DEFINE_H
#define FUNC_DEFINE_H

#include "Element.h"

class Param;
class CompoundStatement;

class FuncDefine: public Element
{
public:
    FuncDefine(){}
    virtual ~FuncDefine(){}

    sym::SYMBOL return_type;
    string func_name;
    vector<Param*> param_list;
    CompoundStatement *compound_state;

    virtual Tuples dump(NameTable &tab){
        Tuples tuples;

        // dump param_list
        vector<VarEntry*> param_entry_list;
        for(Param *param: param_list){
            VarEntry *param_entry = param->dump(tab, func_name);
            if(param_entry != NULL)
                param_entry_list.push_back(param_entry);
        }

        // create a tuple as the entrance
        Tuple *start_tuple = new Tuple();
        start_tuple->op = sem::LABEL;
        start_tuple->left = new Operand(NameUtil::genFuncLabel(func_name));
        tuples.insert(tuples.begin(), start_tuple); // should be at the head

        // fill tab
        if(!tab.insertFunc(func_name, return_type, param_entry_list, start_tuple))
            errorRepo("multiple defination for function: " + func_name);

        // dump compound_state
        Tuples state_tuples = compound_state->dump(tab, func_name);
        tuples.insert(tuples.end(), state_tuples.begin(), state_tuples.end());

        return tuples;
    }
};

#endif//FUNC_DEFINE_H