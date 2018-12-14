#ifndef DAG_WORKER_H
#define DAG_WORKER_H

#include "Builder.h"
#include "BasicBlock.h"

#include <stack>

namespace dag{

    class Worker{
    public:
        static void work(NameTable &tab, BasicBlock *block){
            Tuples head_tuples;
            Tuples mid_tuples;
            Tuple *end_tuple;

            // split tuples as head, mid, tail
            bool head_done = false;
            for(Tuple *tuple: block->tuples){
                if(BasicBlock::isLabelTuple(tuple) ||  BasicBlock::isFuncTuple(tuple)){
                    if(head_done)
                        throw string("dag::Worker: though head_done is true, we still get LabelTuple or FuncTuple.");
                    head_tuples.push_back(tuple);
                    continue;
                }

                head_done = true;
                mid_tuples.push_back(tuple);
            }

            end_tuple = mid_tuples.back();
            if(!BasicBlock::isBranchTuple(end_tuple)
                && !BasicBlock::isJumpTuple(end_tuple)
                && !BasicBlock::isCallTuple(end_tuple)
                && !BasicBlock::isReturnTuple(end_tuple)){
                end_tuple = NULL;
            }
            else{
                mid_tuples.pop_back();
            }

            // build DAG graph
            Builder builder;
            vector<Node*> nodes = builder.work(tab, mid_tuples);

            // build DAG node stack
            stack<Node*> node_stack;
            while(nodes.size() > 0){
                // search node without any father node
                Node *candi = NULL;
                for(auto it=nodes.begin();it!=nodes.end();it++){
                    if((*it)->fathers.size() == 0){
                        candi = *it;
                        nodes.erase(it);
                        break;
                    }
                }
                if(candi == NULL)
                    throw string("dag::Worker: When build DAG node stack, cannot find node without any father node.");

                // if is a OpNode with son, remove the node from son's fathers list.
                OpNode *op_candi = dynamic_cast<OpNode*>(candi);
                if(op_candi){
                    op_candi->removeFromSon();
                }

                // push to node stack
                node_stack.push(candi);
            }

            // dump DAGed mid_tuples
            mid_tuples.clear();
            while(node_stack.size() > 0){
                Node *node = node_stack.top();
                node_stack.pop();
                Tuples tuples = node->dumpTuple();
                mid_tuples.insert(mid_tuples.end(), tuples.begin(), tuples.end());
            }

            // merge head, mid, tail
            block->tuples = head_tuples;
            block->tuples.insert(block->tuples.end(), mid_tuples.begin(), mid_tuples.end());
            if(end_tuple)
                block->tuples.push_back(end_tuple);
        }
    };

}

#endif//DAG_WORKER_H
