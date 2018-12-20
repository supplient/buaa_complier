#ifndef FLOW_TUPLE_H
#define FLOW_TUPLE_H

#include "Tuple.h"

#include <map>

class FlowTuple{
public:
    FlowTuple(Tuple *tuple)
        :tuple(tuple)
    {
        // TODO
    }

    string toString(string tab=""){
        // TODO
        return tuple->toString();
    }

    vector<const VarEntry*> use(){
        vector<const VarEntry*> res;
        // TODO
        return res;
    }

    vector<const VarEntry*> def(){
        vector<const VarEntry*> res;
        // TODO
        return res;
    }

    Tuple *tuple;

    // TODO fill these map
    map<const VarEntry*, FlowTuple*> live_map;
    map<const VarEntry*, FlowTuple*> active_map;
};

#endif//FLOW_TUPLE_H