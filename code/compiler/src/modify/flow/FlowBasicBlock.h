#ifndef FLOW_BASIC_BLOCK
#define FLOW_BASIC_BLOCK

#include <vector>
using namespace std;

#include "FlowTuple.h"
#include "Edge.h"
#include "BasicBlock.h"

class FlowBasicBlock{
public:
    FlowBasicBlock(BasicBlock *block)
        :index(block->index)
    {
        // TODO
    }

    const unsigned int index;
    vector<FlowTuple*> tuples;
    vector<string> labels;

    vector<Edge*> in_edges;
    vector<Edge*> out_edges;
};

#endif//FLOW_BASIC_BLOCK