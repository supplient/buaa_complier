#ifndef DAG_NODE_H
#define DAG_NODE_H

#include "Tuple.h"
#include "symbol.h"

#include <set>

namespace dag{
    class OpNode;

    class Node
    {
    public:
        set<OpNode*> fathers;

        void addFather(OpNode *father){
            fathers.insert(father);
        }
        // TODO why no father

        bool removeFather(OpNode *father){
            auto it = fathers.find(father);
            if(it != fathers.end()){
                fathers.erase(it);
                return true;
            }
            return false;
        }

        virtual Operand* dumpOperand(){
            throw string("dag::Node.dumpOperand: Not Implemented.");
        }

        virtual Tuples dumpTuple(){
            Tuples res; // return empty
            return res;
        }
    };

    class StringNode: public Node
    {
    public:
        string str_value;

        StringNode(string str_value): str_value(str_value){}

        virtual Operand* dumpOperand() override{
            Operand *res = new Operand(str_value);
            return res;
        }
    };

    class SpecialNode: public Node
    {
    public:
        string name;

        SpecialNode(string name): name(name){}

        virtual Operand* dumpOperand() override{
            throw string("dga::SpecialNode[" + name + "]: trying to dumpOperand, which is not allowed for SpecialNode.");
        }
    };

    class VarNode: public Node
    {
    public:
        set<const VarEntry*> vars;

        VarNode(){}

        VarNode(const VarEntry *var){
            vars.insert(var);
        }

        void addVar(const VarEntry *var){
            if(!var)
                throw new string("dag::Node.addVar: var is NULL");
            vars.insert(var);
        }

        virtual Operand* buildDelegate(){
            if(vars.size() < 1)
                throw string("dag::VarNode: vars is empty!");
            // TODO select a var
            return new Operand(*(vars.begin()));
        }

        virtual Operand* dumpOperand() override{
            return buildDelegate();
        }

        virtual Tuples dumpTuple(){
            Tuples tuples;

            for(const VarEntry *var: vars){
                Operand *left = buildDelegate();
                if(left->type == Operand::ENTRY && left->entry == var){
                    delete left;
                    continue;
                }

                Tuple *tuple = new Tuple();
                tuple->op = sem::ASSIGN;
                tuple->res = new Operand(var);
                tuple->left = left;
                tuples.push_back(tuple);
            }

            return tuples;
        }
    };

    class ValueNode: public VarNode
    {
    public:
        sym::SYMBOL type;
        int int_value;
        char char_value;

        ValueNode(int int_value): type(sym::INT), int_value(int_value){}
        ValueNode(char char_value): type(sym::CHAR), char_value(char_value){}

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

        Node *left = NULL;
        Node *mid = NULL;
        Node *right = NULL;
        Node *special = NULL;

        OpNode(){}
        OpNode(sem::OP op, Node *left): op(op), left(left){
            left->addFather(this);
        }
        OpNode(sem::OP op, Node *left, Node *right): op(op), left(left), right(right){
            left->addFather(this);
            right->addFather(this);
        }
        OpNode(sem::OP op, Node *left, Node *mid, Node *right): op(op), left(left), mid(mid), right(right){
            left->addFather(this);
            mid->addFather(this);
            right->addFather(this);
        }

        OpNode(Node *special, sem::OP op, Node *left, Node *right): op(op), left(left), right(right), special(special){
            special->addFather(this);
            left->addFather(this);
            right->addFather(this);
        }

        virtual Tuples dumpTuple(){
            Tuples tuples;

            Tuple *tuple = new Tuple();
            tuple->op = op;
            switch(op){
                case sem::ADD:
                    tuple->res = buildDelegate();
                    tuple->left = left->dumpOperand();
                    tuple->right = right->dumpOperand();
                    break;

                case sem::OUTPUT:
                    if(left)
                        tuple->left = left->dumpOperand();
                    if(right)
                        tuple->right = right->dumpOperand();
                    break;

                default:
                    throw string("dag::OpNode.dumpTuple: Not Implemented sem::op:" + to_string(op));
            }
            tuples.push_back(tuple);

            // Merge assign tuples
            Tuples assign_tuples = VarNode::dumpTuple();
            tuples.insert(tuples.end(), assign_tuples.begin(), assign_tuples.end());

            return tuples;
        }
    };
}

#endif//DAG_NODE_H
