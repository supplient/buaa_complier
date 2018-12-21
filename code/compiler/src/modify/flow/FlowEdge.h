#ifndef FLOW_EDGE_H
#define FLOW_EDGE_H

#include "Edge.h"

class FlowBasicBlock;

class FlowEdge{
public:
    FlowEdge(FlowBasicBlock *from, FlowBasicBlock *to, Edge::TYPE type)
        :from(from), to(to), type(type)
    {}

    FlowBasicBlock *from;
    FlowBasicBlock *to;
    Edge::TYPE type;
};

#endif//FLOW_EDGE_H
