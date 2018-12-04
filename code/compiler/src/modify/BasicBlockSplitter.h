#ifndef BASIC_BLOCK_SPLITTER_H
#define BASIC_BLOCK_SPLITTER_H

#include "FuncBlock.h"
#include "FuncTuple.h"

#include <algorithm>

class BasicBlockSplitter
{
public:
    static vector<FuncBlock*> work(vector<FuncTuple*> func_tuples){
        vector<FuncBlock*> func_blocks;
        for(FuncTuple *func_tuple: func_tuples){
            func_blocks.push_back(workOnFunc(func_tuple));
        }
        return func_blocks;
    }

    static FuncBlock* workOnFunc(FuncTuple *func_tuple){
        FuncBlock *func_block = new FuncBlock(func_tuple->func_entry);
        unsigned int index = 0;
        BasicBlock *block = new BasicBlock(index++);
        bool has_started = false;

        func_block->blocks.push_back(block);

        // split and construct basic blocks
        // link normal edge
        for(Tuple *tuple: func_tuple->tuples){
            if(BasicBlock::isLabelTuple(tuple) || BasicBlock::isFuncTuple(tuple)){
                if(!has_started){
                    block->labels.push_back(tuple->res->str_value);
                    block->tuples.push_back(tuple);
                }
                else{
                    has_started = false;
                    BasicBlock *new_block = new BasicBlock(index++);
                    func_block->blocks.push_back(new_block);
                    new_block->labels.push_back(tuple->res->str_value);
                    new_block->tuples.push_back(tuple);
                    addEdgeBetween(block, new_block, Edge::NORMAL);
                    block = new_block;
                }
                if(BasicBlock::isFuncTuple(tuple))
                    func_block->enter_block = block;
                continue;
            }

            has_started = true;

            if(BasicBlock::isJumpTuple(tuple) || BasicBlock::isBranchTuple(tuple) || BasicBlock::isCallTuple(tuple)){
                block->tuples.push_back(tuple);
                has_started = false;
                BasicBlock *new_block = new BasicBlock(index++);
                func_block->blocks.push_back(new_block);
                if(BasicBlock::isBranchTuple(tuple) || BasicBlock::isCallTuple(tuple))
                    addEdgeBetween(block, new_block, Edge::NORMAL);
                block = new_block;
            }
            else if(BasicBlock::isReturnTuple(tuple)){
                block->tuples.push_back(tuple);
                has_started = false;
                BasicBlock *new_block = new BasicBlock(index++);
                func_block->blocks.push_back(new_block);
                func_block->exit_blocks.push_back(block);
                block = new_block;
            }
            else
                block->tuples.push_back(tuple);
        }//for(Tuple *tuple: func_tuple->tuples){

        // Remove the last empty block
        block = func_block->blocks.back();
        delete block;
        func_block->blocks.pop_back();

        // link jmp edges and branch edges
        for(BasicBlock *block: func_block->blocks){
            if(block->tuples.size() < 1)
                continue;
            Tuple *last_tuple = block->tuples.back();
            if(BasicBlock::isJumpTuple(last_tuple) || BasicBlock::isBranchTuple(last_tuple)){
                string to_label = last_tuple->res->str_value;
                BasicBlock *to_block = searchForBlockWithLabel(func_block->blocks, to_label);
                if(to_block == NULL)
                    throw string("BasicBlockSplitter: get a NULL when search for block with label [" + to_label + "]");

                Edge::TYPE edge_type;
                if(BasicBlock::isJumpTuple(last_tuple))
                    edge_type = Edge::JUMP;
                else// isBranchTuple
                    edge_type = Edge::BRANCH;

                addEdgeBetween(block, to_block, edge_type);
            }
            else
                continue;
        }

        return func_block;
    }

private:
    static BasicBlock *searchForBlockWithLabel(const vector<BasicBlock*> &blocks, string label){
        for(BasicBlock *block: blocks){
            auto it = find(block->labels.begin(), block->labels.end(), label);
            if(it != block->labels.end())
                return block;
        }
        return NULL;
    }

    static void addEdgeBetween(BasicBlock *from, BasicBlock *to, Edge::TYPE type){
        Edge *edge = new Edge(from, to, type);
        from->out_edges.push_back(edge);
        to->in_edges.push_back(edge);
    }
};

#endif//BASIC_BLOCK_SPLITTER_H
