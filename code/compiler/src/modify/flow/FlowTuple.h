#ifndef FLOW_TUPLE_H
#define FLOW_TUPLE_H

#include "Tuple.h"

class FlowTuple{
public:
    FlowTuple(Tuple *tuple)
        :tuple(tuple)
    {
        // TODO
    }

    Tuple *tuple;
};

#endif//FLOW_TUPLE_H