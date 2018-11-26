#ifndef FUNC_TUPLE_H
#define FUNC_TUPLE_H

#include "Tuple.h"

class FuncTuple
{
public:
    FuncTuple(const Tuples &tuples, const string &func_name)
        :tuples(tuples), func_name(func_name)
    {}

    string toString(){
        string s = "";
        for(Tuple *tuple: tuples){
            s += tuple->toString();
            s += "\n";
        }
        return s;
    }

    const Tuples tuples;
    const string func_name;
};

#endif//FUNC_TUPLE_H