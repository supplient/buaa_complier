#ifndef DAG_BUILDER_H
#define DAG_BUILDER_H

#include "Node.h"

#include <map>

namespace dag{
    class Builder
    {
    public:
        Builder(){
            reset();
        }

        // TODO free tuples(outside)
        vector<Node*> work(Tuples tuples){
            // In: should be tuples in a basic block
            //      without head's label tuples
            //      and tail's jump tuples
            // Out: is the DAG tree's all nodes
            reset();

            Node *left;
            Node *mid;
            Node *right;
            Node *special;

            VarNode *var_node;
            OpNode *op_node;

            for(Tuple *tuple: tuples){

                switch(tuple->op){
                    case sem::ASSIGN:
                        left = getNodeAndFillTab(tuple->left);
                        var_node = dynamic_cast<VarNode*>(left);
                        if(!var_node)
                            throw string("dag::Builder: When processing ASSIGN, its left ord is not mapping a VarNode.");
                        var_node->addVar(tuple->res->entry);
                        var_tab[tuple->res->entry] = left;
                        break;

                    case sem::ADD:
                        left = getNodeAndFillTab(tuple->left);
                        right = getNodeAndFillTab(tuple->right);
                        op_node = getOpNodeWithTwo(tuple->op, left, right, false);
                        op_node->addVar(tuple->res->entry);
                        var_tab[tuple->res->entry] = op_node;
                        break;

                    case sem::OUTPUT:
                        left = NULL;
                        right = NULL;
                        if(tuple->left){
                            left = getNodeAndFillTab(tuple->left);
                        }
                        if(tuple->right){
                            right = getNodeAndFillTab(tuple->right);
                        }
                        op_node = new OpNode(out_node, sem::OUTPUT, left, right);
                        nodes.push_back(op_node);
                        out_node = op_node;
                        break;
                    // TODO

                    default:
                        throw string("dag::Builder.work: Not Implemented sem::op: " + to_string(tuple->op));
                }

            }

            return nodes;
        }

    private:
        map<const VarEntry*, Node*> var_tab;
        map<int, Node*> int_tab;
        map<char, Node*> char_tab;
        map<string, Node*> str_tab;

        Node* stack_node;
        Node* in_node;
        Node* out_node;

        vector<Node*> nodes;

        void reset(){
            // TODO free memory
            var_tab.clear();
            int_tab.clear();
            char_tab.clear();
            str_tab.clear();
            stack_node = new SpecialNode("stack");
            in_node = new SpecialNode("in");
            out_node = new SpecialNode("out");
            nodes.clear();
        }

        OpNode* getOpNodeWithTwo(sem::OP op, Node *left, Node *right, bool order){
            OpNode *res = NULL;

            for(OpNode *node: left->fathers){
                if(node->op != op)
                    continue;

                if(order){
                    if(node->left != left)
                        continue;
                    if(node->right != right)
                        continue;
                }
                else{
                    if(node->left == left){
                        if(node->right != right)
                            continue;
                    }
                    else if(node->right == left){
                        if(node->left != right)
                            continue;
                    }
                    else
                        throw string("dag::Builder.getOpNodeWithTwo: though the op node is in left's fathers, left != node.left && left != node.right");
                }

                return node;
            }

            res = new OpNode(op, left, right);
            nodes.push_back(res);

            return res;
        }

        Node* getNodeAndFillTab(Operand *ord){
            auto var_it = var_tab.end();
            auto int_it = int_tab.end();
            auto char_it = char_tab.end();
            auto str_it = str_tab.end();

            Node *res = NULL;
            string entry_name;

            switch(ord->type){
                case Operand::ENTRY:
                    var_it = var_tab.find(ord->entry);
                    if(var_it == var_tab.end()){
                        res = new VarNode(ord->entry);
                        var_tab[ord->entry] = res;
                        nodes.push_back(res);
                    }
                    else
                        res = var_it->second;
                    break;
                case Operand::INT_CONST:
                    int_it = int_tab.find(ord->int_const);
                    if(int_it == int_tab.end()){
                        res = new ValueNode(ord->int_const);
                        int_tab[ord->int_const] = res;
                        nodes.push_back(res);
                    }
                    else
                        res = int_it->second;
                    break;
                case Operand::CHAR_CONST:
                    char_it = char_tab.find(ord->char_const);
                    if(char_it == char_tab.end()){
                        res = new ValueNode(ord->char_const);
                        char_tab[ord->char_const] = res;
                        nodes.push_back(res);
                    }
                    else
                        res = char_it->second;
                    break;
                case Operand::STRING:
                    str_it = str_tab.find(ord->str_value);
                    if(str_it == str_tab.end()){
                        res = new StringNode(ord->str_value);
                        str_tab[ord->str_value] = res;
                        nodes.push_back(res);
                    }
                    else
                        res = str_it->second;
                    break;
                default:
                    throw string("dag::Builder.getNodeAndFillTab: Invalid Operand.type: " + to_string(ord->type));
            }

            return res;
        }
    };
}
#endif//DAG_BUILDER_H
