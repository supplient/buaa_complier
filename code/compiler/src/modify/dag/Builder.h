#ifndef DAG_BUILDER_H
#define DAG_BUILDER_H

#include "Node.h"
#include "log.h"
#include "NameTable.h"
#include "VarPool.h"

#include <map>

namespace dag{
    class Builder
    {
    public:
        Builder(){
            reset();
        }

        vector<Node*> work(FuncNameTable *func_tab, Tuples tuples){
            // In: should be tuples in a basic block
            //      without head's label tuples
            //      and tail's jump tuples
            // Out: is the DAG tree's all nodes
            reset();
            dag_var_pool = new VarPool(func_tab);

            Node *left;
            Node *mid;
            Node *right;

            OpNode *op_node;

            // Build DGA graph
            for(Tuple *tuple: tuples){

                switch(tuple->op){
                    case sem::ASSIGN:
                        left = getNodeAndFillTab(tuple->left);
                        assignVarToNode(func_tab, left, tuple->res->entry);
                        break;

                    case sem::NEG:
                        left = getNodeAndFillTab(tuple->left);
                        op_node = getOpNodeWithOne(tuple->op, left);
                        assignVarToNode(func_tab, op_node, tuple->res->entry);
                        break;

                    case sem::RARRAY:
                    case sem::SUB:
                    case sem::DIV:
                    case sem::LESS:
                    case sem::LESSOREQUAL:
                    case sem::MORE:
                    case sem::MOREOREQUAL:
                        left = getNodeAndFillTab(tuple->left);
                        right = getNodeAndFillTab(tuple->right);
                        op_node = getOpNodeWithTwo(tuple->op, left, right, true);
                        assignVarToNode(func_tab, op_node, tuple->res->entry);
                        break;

                    case sem::ADD:
                    case sem::MUL:
                    case sem::NOTEQUAL:
                    case sem::EQUAL:
                        left = getNodeAndFillTab(tuple->left);
                        right = getNodeAndFillTab(tuple->right);
                        op_node = getOpNodeWithTwo(tuple->op, left, right, false);
                        assignVarToNode(func_tab, op_node, tuple->res->entry);
                        break;

                    case sem::WARRAY:
                        left = getNodeAndFillTab(tuple->left);
                        mid = getNodeAndFillTab(tuple->res);
                        right = getNodeAndFillTab(tuple->right);
                        op_node = new OpNode(tuple->op, left, mid, right);
                        nodes.push_back(op_node);
                        op_node->addSubVar(tuple->res->entry);
                        var_tab[tuple->res->entry] = op_node;
                        break;

                    case sem::PARAM:
                        left = getNodeAndFillTab(tuple->left);
                        mid = getNodeAndFillTab(tuple->res);
                        right = getNodeAndFillTab(tuple->right);
                        op_node = new OpNode(stack_node, param_node, tuple->op, left, mid, right);
                        nodes.push_back(op_node);
                        stack_node = op_node;
                        param_node = op_node;
                        break;

                    case sem::INPUT:
                        op_node = new OpNode(stream_node, tuple->op);
                        nodes.push_back(op_node);
                        assignVarToNode(func_tab, op_node, tuple->res->entry);
                        stream_node = op_node;
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
                        op_node = new OpNode(stream_node, param_node, sem::OUTPUT, left, right);
                        nodes.push_back(op_node);
                        stream_node = op_node;
                        param_node = op_node;
                        break;

                    default:
                        throw string("dag::Builder.work: Not Implemented sem::op: " + to_string(tuple->op));
                }

            }

            // DEBUG
            mylog::debug << "Built DAG graph-----";
            for(Node *node: nodes)
                mylog::debug << node->toString();

            return nodes;
        }

    private:
        map<const VarEntry*, VarNode*> var_tab;
        map<int, Node*> int_tab;
        map<char, Node*> char_tab;
        map<string, Node*> str_tab;

        Node* param_node;
        Node* stack_node;
        Node* stream_node;

        vector<Node*> nodes; // Note: we must ensure node is inserted into nodes in its creating order to maintain old_ref

        VarPool *dag_var_pool = NULL;

        void reset(){
            // TODO free memory
            var_tab.clear();
            int_tab.clear();
            char_tab.clear();
            str_tab.clear();

            param_node = new SpecialNode("param");
            stack_node = new SpecialNode("stack");
            stream_node = new SpecialNode("stream");

            nodes.clear();

            if(dag_var_pool)
                delete dag_var_pool;
            dag_var_pool = NULL;
        }

        void assignVarToNode(FuncNameTable *func_tab, Node *node, const VarEntry *var){
            VarNode *var_node = dynamic_cast<VarNode*>(node);
            if(!var_node)
                throw string("dag::Builder.assignVarToNode: the node cannot be converted to VarNode.");

            auto it = var_tab.find(var);
            if(it == var_tab.end()){
                // if not used before, just fill var_tab & addSubVar
                var_node->addSubVar(var);
                var_tab[var] = var_node;
                return;
            }

            // if is used before, we need to use a new var to save old value
            VarNode *old_node = var_tab[var];

            // check dim, because array is not assignable
            if(var->dim > 0)
                throw string("dag::Builder.assignVarToNode: trying to assign an array to a node.");

            // create a new var
            VarEntry *new_var = dag_var_pool->getNewTempVar(var->type);

            // replace old node's main_var or sub_var
            if(old_node->main_var == var){
                old_node->replaceMainAndSetOrigin(new_var);
            }
            else{
                old_node->removeSubVar(var);
                old_node->addSubVar(new_var);
            }

            // finally fill var_tab & addSubVar
            var_node->addSubVar(var);
            var_tab[var] = var_node;
        }

        OpNode* getOpNodeWithOne(sem::OP op, Node *left){
            for(OpNode *node: left->fathers){
                if(node->op != op)
                    continue;

                if(node->left != left)
                    continue;

                return node;
            }

            OpNode *res = NULL;
            res = new OpNode(op, left);
            nodes.push_back(res);

            return res;
        }

        OpNode* getOpNodeWithTwo(sem::OP op, Node *left, Node *right, bool order){
            if(DAG_ALWAYS_ORDER)
                order = true;

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

            OpNode *res = NULL;
            res = new OpNode(op, left, right);
            nodes.push_back(res);

            return res;
        }

        OpNode* getOpNodeWithThree_order(sem::OP op, Node *left, Node *mid, Node *right){
            for(OpNode *node: left->fathers){
                if(node->op != op)
                    continue;
                if(node->left != left
                    || node->mid != mid
                    || node->right != right
                )
                    continue;

                return node;
            }

            OpNode *res = NULL;
            res = new OpNode(op, left, mid, right);
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
                        var_tab[ord->entry] = new VarNode(ord->entry);
                        res = var_tab[ord->entry];
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
