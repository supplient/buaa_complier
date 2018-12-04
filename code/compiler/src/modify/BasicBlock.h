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
        for(Edge *edge: out_edges){
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

    static bool isLabelTuple(Tuple *tuple){
        return tuple->op==sem::LABEL;
    }

    static bool isFuncTuple(Tuple *tuple){
        return tuple->op==sem::FUNC;
    }

    static bool isJumpTuple(Tuple *tuple){
        return tuple->op==sem::JMP;
    }

    static bool isBranchTuple(Tuple *tuple){
        switch(tuple->op){
            case sem::BEQ:
            case sem::BEZ:
                return true;
            default:
                return false;
        }
    }

    static bool isCallTuple(Tuple *tuple){
        return tuple->op == sem::CALL;
    }

    static bool isReturnTuple(Tuple *tuple){
        return tuple->op == sem::RET;
    }
};

#endif//BASIC_BLOCK_H
