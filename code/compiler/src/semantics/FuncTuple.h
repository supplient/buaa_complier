#ifndef FUNC_TUPLE_H
#define FUNC_TUPLE_H

#include "Tuple.h"
#include "NameTableEntry.h"

class NameTable;

class FuncTuple
{
public:
    FuncTuple(const Tuples &tuples, const FuncEntry *func_entry)
        :tuples(tuples), func_entry(func_entry)
    {
        if(!func_entry)
            throw string("FuncTuple: func_entry is NULL.");
    }

    string toString(NameTable &tab){
        string s = "";
        for(Tuple *tuple: tuples){
            s += tuple->toString(tab);
            s += "\n";
        }
        return s;
    }

    string toString(){
        string s = "";
        for(Tuple *tuple: tuples){
            s += tuple->toString();
            s += "\n";
        }
        return s;
    }

    const Tuples tuples;
    const FuncEntry* func_entry;
};

#endif//FUNC_TUPLE_H
