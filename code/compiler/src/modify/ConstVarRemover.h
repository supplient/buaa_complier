#ifndef CONST_VAR_REMOVER_H
#define CONST_VAR_REMOVER_H

#include "FuncTuple.h"
#include "NameTable.h"

class ConstVarRemover
{
public:
    static void work(NameTable &tab, vector<FuncTuple*> &func_tuples){
        for(auto func_tuple: func_tuples)
            ConstVarRemover::workOnFuncTuple(func_tuple);

        // remove const var from tab
        tab.removeConstVar();
    }

private:
    static void workOnFuncTuple(FuncTuple *func_tuple){
        for(Tuple *tuple: func_tuple->tuples)
            ConstVarRemover::workOnTuple(tuple);
    }

    static void workOnTuple(Tuple *tuple){
        if(tuple->left)
            tuple->left = workOnOperand(tuple->left);
        if(tuple->right)
            tuple->right = workOnOperand(tuple->right);
        if(tuple->res)
            tuple->res = workOnOperand(tuple->res);
    }

    static Operand* workOnOperand(Operand *ord){
        if(ord->type != Operand::ENTRY)
            return ord;

        const NameTableEntry *entry = ord->entry;
        if(entry->entry_type != sem::VAR_ENTRY)
            return ord;

        const VarEntry *var_entry = dynamic_cast<const VarEntry*>(entry);
        if(!var_entry->is_const)
            return ord;

        Operand *new_ord = NULL;
        switch(var_entry->type){
            case sym::INT:
                new_ord = new Operand(var_entry->int_value);
                break;
            case sym::CHAR:
                new_ord = new Operand(var_entry->char_value);
                break;
            default:
                throw string("ConstVarRemover: Invalid type for varEntry: " + to_string(var_entry->type));
        }

        if(!new_ord)
            throw string("ConstVarRemover: new_ord is NULL!");
        return new_ord;
    }
};

#endif//CONST_VAR_REMOVER_H