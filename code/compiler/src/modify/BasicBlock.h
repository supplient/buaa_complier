#ifndef BASIC_BLOCK_H
#define BASIC_BLOCK_H

#include "Tuple.h"
#include "Edge.h"

class BasicBlock
{
public:
    BasicBlock(unsigned int index):index(index){}

    string toString(string tab=""){
        string s = tab + "[Basic Block " + to_string(index) + "]\n";

        s += tab + "[IN";
        for(Edge *edge: in_edges){
            s += "  " + to_string(edge->from->index);
            switch(edge->type){
                case Edge::NORMAL:
                    s += "N";
                    break;
                default:
                    throw string("Invaliad Edge's type: " + to_string(edge->type));
            }
        }
        s += "]\n";

        s += tab + "[OUT";
        for(Edge *edge: out_edges){
            s += "  " + to_string(edge->to->index);
            switch(edge->type){
                case Edge::NORMAL:
                    s += "N";
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

        for(Tuple *tuple: tuples){
            s += tab + tuple->toString() + "\n";
        }

        return s;
    }

    const unsigned int index;
    Tuples tuples;
    vector<string> labels;

    vector<Edge*> in_edges;
    vector<Edge*> out_edges;
};

#endif//BASIC_BLOCK_H
