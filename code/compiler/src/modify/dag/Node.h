#ifndef DAG_NODE_H
#define DAG_NODE_H

#include "Tuple.h"
#include "symbol.h"
#include "NameTableEntry.h"

#include <set>

namespace dag{
    class OpNode;

    class Node
    {
    public:
        set<OpNode*> fathers;
        unsigned int index;

        Node(){
            static unsigned int node_count = 0;
            index = node_count++;
        }

        void addFather(OpNode *father){
            fathers.insert(father);
        }

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

        virtual string toString();
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

        virtual string toString(){
            string s = Node::toString();
            s += "\tString: " + str_value + "\n";
            return s;
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

        virtual string toString(){
            string s = Node::toString();
            s += "\t Special name: " + name + "\n";
            return s;
        }
    };

    class VarNode: public Node
    {
    public:
        const VarEntry* var = NULL;

        VarNode(){}

        VarNode(const VarEntry *var){
            setVar(var);
        }

        void setVar(const VarEntry *var){
            if(!var)
                throw new string("dag::Node.setVar: var is NULL");
            this->var = var;
        }

        virtual Operand* dumpOperand() override{
            if(!var)
                throw string("dag::VarNode: var is NULL!");
            return new Operand(var);
        }

        virtual string toString(){
            string s = Node::toString();
            if(var)
                s += "\tVar: " + NameUtil::genEntryName(var) + "\n";
            return s;
        }
    };

    class ValueNode: public Node
    {
    public:
        sym::SYMBOL type;
        int int_value;
        char char_value;

        ValueNode(int int_value): type(sym::INT), int_value(int_value){}
        ValueNode(char char_value): type(sym::CHAR), char_value(char_value){}

        virtual Operand* dumpOperand() override{
            switch(type){
                case sym::INT: return new Operand(int_value);
                case sym::CHAR: return new Operand(char_value);
                default: throw string("dag::ValueNode.buildDelegate: Invalid value type: " + to_string(type));
            }
        }

        virtual string toString(){
            string s = Node::toString();
            if(type == sym::INT)
                s += "\tInt value: " + to_string(int_value);
            else
                s += "\tChar value: " + string(1, char_value);
            return s + "\n";
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
            addFatherToSon();
        }
        OpNode(sem::OP op, Node *left, Node *right): op(op), left(left), right(right){
            addFatherToSon();
        }
        OpNode(sem::OP op, Node *left, Node *mid, Node *right): op(op), left(left), mid(mid), right(right){
            addFatherToSon();
        }

        OpNode(Node *spe_1, sem::OP op): op(op), spe_1(spe_1){
            addFatherToSon();
        }

        OpNode(Node *spe_1, Node *spe_2, sem::OP op, Node *left, Node *right): op(op), left(left), right(right), spe_1(spe_1), spe_2(spe_2){
            addFatherToSon();
        }

        OpNode(Node *spe_1, Node *spe_2, sem::OP op, Node *left, Node *mid, Node *right): op(op), left(left), mid(mid), right(right), spe_1(spe_1), spe_2(spe_2){
            addFatherToSon();
        }

        void removeFromSon(){
            if(left)
                left->removeFather(this);
            if(mid)
                mid->removeFather(this);
            if(right)
                right->removeFather(this);
            if(spe_1)
                spe_1->removeFather(this);
            if(spe_2)
                spe_2->removeFather(this);
        }

        virtual Tuples dumpTuple(){
            Tuples tuples;

            Tuple *tuple = new Tuple();
            tuple->op = op;
            switch(op){
                case sem::NEG:
                    tuple->res = this->dumpOperand();
                    tuple->left = left->dumpOperand();
                    break;

                case sem::ASSIGN:
                    tuple->res = this->dumpOperand();
                    tuple->left = left->dumpOperand();
                    break;

                case sem::RARRAY:
                case sem::ADD:
                case sem::SUB:
                case sem::DIV:
                case sem::LESS:
                case sem::LESSOREQUAL:
                case sem::MORE:
                case sem::MOREOREQUAL:
                case sem::MUL:
                case sem::NOTEQUAL:
                case sem::EQUAL:
                    tuple->res = this->dumpOperand();
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
                    tuple->res = this->dumpOperand();
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

        virtual string toString(){
            string s = VarNode::toString();
            s += "\tOP: " + string(sem::TUPLE_OP_NAME[op]) + "\n";
            if(left)
                s += "\tleft: " + to_string(left->index) + "\n";
            if(mid)
                s += "\tmid: " + to_string(mid->index) + "\n";
            if(right)
                s += "\tright: " + to_string(right->index) + "\n";
            if(spe_1)
                s += "\tspe_1: " + to_string(spe_1->index) + "\n";
            if(spe_2)
                s += "\tspe_2: " + to_string(spe_2->index) + "\n";
            return s;
        }

    private:
        void addFatherToSon(){
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
