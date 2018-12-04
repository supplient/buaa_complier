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

                if(var->dim > 0)
                    throw string("dag::VarNode.dumpTuple: trying to assign arrays.");
                if(left->type == Operand::ENTRY){
                    if(left->entry->dim > 0)
                        throw string("dag::VarNode.dumpTuple: trying to assign arrays.");
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
        Node *spe_1 = NULL;
        Node *spe_2 = NULL;

        OpNode(){}
        OpNode(sem::OP op, Node *left): op(op), left(left){
            addFather();
        }
        OpNode(sem::OP op, Node *left, Node *right): op(op), left(left), right(right){
            addFather();
        }
        OpNode(sem::OP op, Node *left, Node *mid, Node *right): op(op), left(left), mid(mid), right(right){
            addFather();
        }

        OpNode(Node *spe_1, sem::OP op): op(op), spe_1(spe_1){
            addFather();
        }

        OpNode(Node *spe_1, Node *spe_2, sem::OP op, Node *left, Node *right): op(op), left(left), right(right), spe_1(spe_1), spe_2(spe_2){
            addFather();
        }

        OpNode(Node *spe_1, Node *spe_2, sem::OP op, Node *left, Node *mid, Node *right): op(op), left(left), mid(mid), right(right), spe_1(spe_1), spe_2(spe_2){
            addFather();
        }

        virtual Tuples dumpTuple(){
            Tuples tuples;

            Tuple *tuple = new Tuple();
            tuple->op = op;
            switch(op){
                case sem::NEG:
                    tuple->res = buildDelegate();
                    tuple->left = left->dumpOperand();
                    break;

                case sem::RARRAY:
                case sem::SUB:
                case sem::DIV:
                case sem::LESS:
                case sem::LESSOREQUAL:
                case sem::MORE:
                case sem::MOREOREQUAL:
                case sem::ADD:
                case sem::MUL:
                case sem::NOTEQUAL:
                case sem::EQUAL:
                    tuple->res = buildDelegate();
                    tuple->left = left->dumpOperand();
                    tuple->right = right->dumpOperand();
                    break;

                case sem::WARRAY:
                case sem::PARAM:
                    tuple->res = mid->dumpOperand();
                    tuple->left = left->dumpOperand();
                    tuple->right = right->dumpOperand();
                    break;

                case sem::INPUT:
                    tuple->res = buildDelegate();
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

    private:
        void addFather(){
            if(left)
                left->addFather(this);
            if(mid)
                mid->addFather(this);
            if(right)
                right->addFather(this);
            if(spe_1)
                spe_1->addFather(this);
            if(spe_2)
                spe_2->addFather(this);
        }
    };
}

#endif//DAG_NODE_H
