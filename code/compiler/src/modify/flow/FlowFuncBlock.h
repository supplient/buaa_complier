#ifndef FLOW_FUNC_BLOCK_H
#define FLOW_FUNC_BLOCK_H

#include "FlowBasicBlock.h"
#include "NameTableEntry.h"

#include "FuncBlock.h"

#include <algorithm>

// TODO release analyzed FuncBlock and BasicBlock

class FlowFuncBlock{
public:
    static FlowFuncBlock* fromFuncBlock(FuncBlock *func_block){
        return new FlowFuncBlock(func_block);
    }

    FlowFuncBlock(FuncBlock *func_block){
        this->func_entry = func_block->func_entry;

        map<BasicBlock*, FlowBasicBlock*> block_map;
        auto it = func_block->exit_blocks.end();
        for(BasicBlock *block: func_block->blocks){
            FlowBasicBlock *flow_block = new FlowBasicBlock(block);
            block_map[block] = flow_block;
            this->blocks.push_back(flow_block);

            if(func_block->enter_block == block)
                this->enter_block = flow_block;

            it = find(func_block->exit_blocks.begin(), func_block->exit_blocks.end(), block);
            if(it != func_block->exit_blocks.end())
                this->exit_blocks.push_back(flow_block);
        }

        // init edges
        for(BasicBlock *block: func_block->blocks){
            FlowBasicBlock *flow_block = block_map[block];
            for(Edge *in_edge: block->in_edges){
                flow_block->in_edges.push_back(
                    new FlowEdge(
                        block_map[in_edge->from],
                        block_map[in_edge->to],
                        in_edge->type
                    )
                );
            }

            for(Edge *out_edge: block->out_edges){
                flow_block->out_edges.push_back(
                    new FlowEdge(
                        block_map[out_edge->from],
                        block_map[out_edge->to],
                        out_edge->type
                    )
                );
            }
        }

    }

    FuncBlock* toFuncBlock(){
        FuncBlock *func_block = new FuncBlock(func_entry);

        map<FlowBasicBlock*, BasicBlock*> block_map;
        auto it = this->exit_blocks.end();
        for(FlowBasicBlock *flow_block: this->blocks){
            BasicBlock *block = flow_block->toBasicBlock();
            block_map[flow_block] = block;
            func_block->blocks.push_back(block);

            if(this->enter_block == flow_block)
                func_block->enter_block = block;

            it = find(this->exit_blocks.begin(), this->exit_blocks.end(), flow_block);
            if(it != this->exit_blocks.end())
                func_block->exit_blocks.push_back(block);
        }

        // init edges
        for(FlowBasicBlock *flow_block: this->blocks){
            BasicBlock *block = block_map[flow_block];
            for(FlowEdge *in_edge: flow_block->in_edges){
                block->in_edges.push_back(
                    new Edge(
                        block_map[in_edge->from],
                        block_map[in_edge->to],
                        in_edge->type
                    )
                );
            }

            for(FlowEdge *out_edge: flow_block->out_edges){
                block->out_edges.push_back(
                    new Edge(
                        block_map[out_edge->from],
                        block_map[out_edge->to],
                        out_edge->type
                    )
                );
            }
        }

        return func_block;
    }

    string toString(){
        string s = "[Flow Func Block " + func_entry->name + "]\n";

        s += "[ENTER\t" + to_string(enter_block->index) + "]\n";

        s += "[EXIT";
        for(FlowBasicBlock *block: exit_blocks){
            s += "  " + to_string(block->index);
        }
        s += "]\n";

        s += "\n";

        for(FlowBasicBlock *block: blocks){
            s += block->toString("\t") + "\n";
        }

        return s;
    }

    FlowBasicBlock *enter_block;
    vector<FlowBasicBlock*> exit_blocks;

    vector<FlowBasicBlock*> blocks;
    const FuncEntry *func_entry;
};

#endif//FLOW_FUNC_BLOCK_H