#ifndef FLOW_BASIC_BLOCK
#define FLOW_BASIC_BLOCK

#include <vector>
using namespace std;

#include "FlowTuple.h"
#include "FlowEdge.h"
#include "Edge.h"
#include "BasicBlock.h"

class FlowBasicBlock{
public:
    FlowBasicBlock(BasicBlock *block)
        :index(block->index)
    {
        for(Tuple *tuple: block->tuples)
            this->tuples.push_back(new FlowTuple(tuple));

        this->labels = block->labels;
    }

    BasicBlock* toBasicBlock(){
        BasicBlock *block = new BasicBlock(index);

        for(FlowTuple *flow_tuple: this->tuples)
            block->tuples.push_back(flow_tuple->tuple);
        block->labels = this->labels;

        return block;
    }

    string toString(string tab=""){
        string s = tab + "[Flow Basic Block " + to_string(index) + "]\n";

        s += tab + "[IN";
        for(FlowEdge *edge: in_edges){
            s += "  " + to_string(edge->from->index);
            switch(edge->type){
                case Edge::NORMAL:
                    s += "N";
                    break;
                case Edge::JUMP:
                    s += "J";
                    break;
                case Edge::BRANCH:
                    s += "B";
                    break;
                default:
                    throw string("Invaliad Edge's type: " + to_string(edge->type));
            }
        }
        s += "]\n";

        s += tab + "[OUT";
        for(FlowEdge *edge: out_edges){
            s += "  " + to_string(edge->to->index);
            switch(edge->type){
                case Edge::NORMAL:
                    s += "N";
                    break;
                case Edge::JUMP:
                    s += "J";
                    break;
                case Edge::BRANCH:
                    s += "B";
                    break;
                default:
                    throw string("Invaliad Edge's type: " + to_string(edge->type));
            }
        }
        s += "]\n";

        s += tab + "[LABEL";
        for(string &s: labels){
            s += "  " + s;
        }
        s += "]\n";

        for(FlowTuple *tuple: tuples){
            s += tab + tuple->toString() + "\n";
        }

        return s;
    }

    const unsigned int index;
    vector<FlowTuple*> tuples;
    vector<string> labels;

    vector<FlowEdge*> in_edges;
    vector<FlowEdge*> out_edges;
};

#endif//FLOW_BASIC_BLOCK