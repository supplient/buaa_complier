#ifndef DAG_NODE_H
#define DAG_NODE_H

#include "Tuple.h"
#include "symbol.h"

namespace dag{
    class Node
    {
    public:
        vector<Node*> fathers;

        virtual Operand* dumpOperand(){
            throw string("dag::Node.dumpOperand: Not Implemented.");
        }

        virtual Tuples dumpTuple(){
            throw string("dag::Node.dumpTuple: Not Implemented.");
        }
    };

    class LeafNode: public Node
    {
    public:
        virtual Tuples dumpTuple() override{
            Tuples res; // return empty
            return res;
        }
    };

    class StringNode: public LeafNode
    {
    public:
        string str_value;

        virtual Operand* dumpOperand() override{
            Operand *res = new Operand(str_value);
            return res;
        }
    };

    class SpecialNode: public LeafNode
    {
    public:
        string name;

        virtual Operand* dumpOperand() override{
            throw string("dga::SpecialNode[" + name + "]: trying to dumpOperand, which is not allowed for SpecialNode.");
        }
    };

    class VarNode: public LeafNode
    {
    public:
        vector<VarEntry*> vars;

        virtual Operand* buildDelegate(){
            if(vars.size() < 1)
                throw string("dag::VarNode: vars is empty!");
            // TODO select a var
            return new Operand(vars[0]);
        }

        virtual Operand* dumpOperand() override{
            return buildDelegate();
        }

        virtual Tuples dumpTuple(){
            Tuples tuples;
            // TODO assign all vars with the same value
            return tuples;
        }
    };

    class ValueNode: public VarNode
    {
    public:
        sym::SYMBOL type;
        int int_value;
        char char_value;

        virtual Operand* buildDelegate() override{
            switch(type){
                case sym::INT: return new Operand(int_value);
                case sym::CHAR: return new Operand(char_value);
                default: throw string("dag::ValueNode.buildDelegate: Invalid value type: " + to_string(type));
            }
        }
    };

    class OpNode: public VarNode
    {
    public:
        sem::OP op;

        Node *left;
        Node *mid;
        Node *right;

        virtual Tuples dumpTuple(){
            Tuples tuples;

            // TODO

            // Merge assign tuples
            Tuples assign_tuples = VarNode::dumpTuple();
            tuples.insert(tuples.end(), assign_tuples.begin(), assign_tuples.end());

            return tuples;
        }
    };
}

#endif//DAG_NODE_H