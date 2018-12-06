#ifndef DAG_BUILDER_H
#define DAG_BUILDER_H

#include "Node.h"
#include "log.h"

#include <map>

namespace dag{
    class Builder
    {
    public:
        Builder(){
            reset();
        }

        vector<Node*> work(Tuples tuples){
            // In: should be tuples in a basic block
            //      without head's label tuples
            //      and tail's jump tuples
            // Out: is the DAG tree's all nodes
            reset();

            Node *left;
            Node *mid;
            Node *right;

            OpNode *op_node;

            // Build DGA graph
            for(Tuple *tuple: tuples){

                switch(tuple->op){
                    case sem::ASSIGN:
                        left = getNodeAndFillTab(tuple->left);
                        createAssignNode(left, tuple->res->entry);
                        break;

                    case sem::NEG:
                        left = getNodeAndFillTab(tuple->left);
                        op_node = getOpNodeWithOne(tuple->op, left);
                        if(op_node)
                            createAssignNode(op_node, tuple->res->entry);
                        else{
                            op_node = new OpNode(tuple->op, left);
                            nodes.push_back(op_node);
                            op_node->setVar(tuple->res->entry);
                            var_tab[tuple->res->entry] = op_node;
                        }
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
                        if(op_node)
                            createAssignNode(op_node, tuple->res->entry);
                        else{
                            op_node = new OpNode(tuple->op, left, right);
                            nodes.push_back(op_node);
                            op_node->setVar(tuple->res->entry);
                            var_tab[tuple->res->entry] = op_node;
                        }
                        break;

                    case sem::ADD:
                    case sem::MUL:
                    case sem::NOTEQUAL:
                    case sem::EQUAL:
                        left = getNodeAndFillTab(tuple->left);
                        right = getNodeAndFillTab(tuple->right);
                        op_node = getOpNodeWithTwo(tuple->op, left, right, false);
                        if(op_node)
                            createAssignNode(op_node, tuple->res->entry);
                        else{
                            op_node = new OpNode(tuple->op, left, right);
                            nodes.push_back(op_node);
                            op_node->setVar(tuple->res->entry);
                            var_tab[tuple->res->entry] = op_node;
                        }
                        break;

                    case sem::WARRAY:
                        left = getNodeAndFillTab(tuple->left);
                        mid = getNodeAndFillTab(tuple->res);
                        right = getNodeAndFillTab(tuple->right);
                        op_node = getOpNodeWithThree_order(tuple->op, left, mid, right);
                        if(op_node)
                            throw string("dag::Builder.WARRAY: a op_node is found, while it is impossible to find a calculated node for array.");
                        op_node = new OpNode(tuple->op, left, mid, right);
                        nodes.push_back(op_node);
                        op_node->setVar(tuple->res->entry);
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
                        op_node->setVar(tuple->res->entry);
                        stream_node = op_node;
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

            reverse(nodes.begin(), nodes.end());
            for(auto pair: var_tab){
                bool back = false;
                for(auto it = nodes.begin(); it!=nodes.end(); it++){
                    if(back){
                        back = false;
                        it--;
                    }

                    Node *node = *it;
                    if(node == pair.second)
                        continue;
                    // is not the final assign
                    if(node->fathers.size() > 0)
                        continue;
                    // has no father

                    OpNode *op_node = dynamic_cast<OpNode*>(node);
                    if(!op_node)
                        continue;
                    // is OpNode
                    if(op_node->var != pair.first)
                        continue;
                    // result is this var
                    if(op_node->op != sem::ASSIGN)
                        continue;
                    // op is assign

                    // is a useless assign node
                    op_node->removeFromSon();
                    it = nodes.erase(it);
                    back = true;

                    if(it == nodes.end())
                        break;
                }
            }
            reverse(nodes.begin(), nodes.end());

            // DEBUG
            mylog::debug << "DAG graph after remove useless OpNodes-----";
            for(Node *node: nodes)
                mylog::debug << node->toString();

            // Add old_ref
            for(Node *node: nodes){
                OpNode *op_node = dynamic_cast<OpNode*>(node);
                if(!op_node)
                    continue;

                // check sons,
                // if son's delegate is a var, which means we are refering its value,
                // add self into son's ref_set
                checkAndAddRef(op_node, op_node->left);
                checkAndAddRef(op_node, op_node->mid);
                checkAndAddRef(op_node, op_node->right);
                    // ignore special

                // check self,
                // if self has any var, which means these vars' value will be updated,
                // add each var's ref into self's old_ref as sons
                // and clear the var's ref_set
                op_node->addOldRef(ref_set_tab[op_node->var]);
                ref_set_tab[op_node->var].clear();
            }

            // DEBUG
            mylog::debug << "DAG graph after add old_ref-----";
            for(Node *node: nodes)
                mylog::debug << node->toString();

            return nodes;
        }

        void checkAndAddRef(OpNode *father, Node *son){
            if(son){
                VarNode *var_node = dynamic_cast<VarNode*>(son);
                if(var_node){
                    Operand *var = var_node->dumpOperand();
                    if(var->type == Operand::ENTRY){
                        ref_set_tab[var->entry].insert(father);
                    }
                    delete var;
                }
            }
        }

    private:
        map<const VarEntry*, Node*> var_tab;
        map<int, Node*> int_tab;
        map<char, Node*> char_tab;
        map<string, Node*> str_tab;

        map<const VarEntry*, set<OpNode*> > ref_set_tab;

        Node* param_node;
        Node* stack_node;
        Node* stream_node;

        vector<Node*> nodes; // Note: we must ensure node is inserted into nodes in its creating order to maintain old_ref

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
        }

        OpNode* createAssignNode(Node *origin_node, const VarEntry *target_var){
            OpNode *op_node = getOpNodeWithOne(sem::ASSIGN, origin_node);
            Node *old_node = origin_node;
            while(op_node){
                if(op_node->var == target_var){
                    var_tab[target_var] = op_node;
                    return NULL;
                }
                old_node = op_node;
                op_node = getOpNodeWithOne(sem::ASSIGN, op_node);
            }

            op_node = new OpNode(sem::ASSIGN, old_node);
            nodes.push_back(op_node);
            op_node->setVar(target_var);
            var_tab[target_var] = op_node;

            return op_node;
        }

        OpNode* getOpNodeWithOne(sem::OP op, Node *left){
            for(OpNode *node: left->fathers){
                if(node->op != op)
                    continue;

                if(node->left != left)
                    continue;

                return node;
            }

            return NULL;
        }

        OpNode* getOpNodeWithTwo(sem::OP op, Node *left, Node *right, bool order){
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

            return NULL;
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

            return NULL;
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
