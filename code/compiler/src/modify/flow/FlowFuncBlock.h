#ifndef FLOW_FUNC_BLOCK_H
#define FLOW_FUNC_BLOCK_H

#include "FlowBasicBlock.h"
#include "NameTableEntry.h"

#include "FuncBlock.h"

#include <algorithm>

// TODO release analyzed FuncBlock and BasicBlock
void mergeVarMap(VarMap &res, const VarMap &from);

class FlowFuncBlock{
public:
    static unsigned int tuple_index;

    static FlowFuncBlock* fromFuncBlock(FuncBlock *func_block){
        return new FlowFuncBlock(func_block);
    }

    FlowFuncBlock(FuncBlock *func_block){
        FlowFuncBlock::tuple_index = 0;

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

        // call analyze
        analyze();
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

    void reset(){
        for(auto block: blocks)
            block->reset();
    }

    void analyze(){
        // TODO check size and remove empty blocks
        reset();
        liveAnalyze();
        activeAnalyze();
    }

    void liveAnalyze(){
        mylog::debug << "Start " + func_entry->name + " 's live analyzing...";

        bool has_change;
        do{
            has_change = false;

            for(FlowBasicBlock *block: blocks){
                vector<FlowBasicBlock*> front_blocks;
                vector<FlowBasicBlock*> back_blocks;
                for(FlowEdge *edge: block->in_edges)
                    front_blocks.push_back(edge->from);
                for(FlowEdge *edge: block->out_edges)
                    back_blocks.push_back(edge->to);

                // cal the block's in
                VarMap cur_flow;
                for(auto front_block: front_blocks)
                    mergeVarMap(cur_flow, front_block->live_map);

                // anlayze tuple by tuple
                for(auto tuple: block->tuples){
                    // assign flow
                    tuple->live_map = cur_flow;

                    // update flow
                    for(const VarEntry *var: tuple->def()){
                        cur_flow[var].clear();
                        cur_flow[var].push_back(tuple);
                    }
                }

                // detect change
                if(!has_change){
                    if(block->live_map.size() != cur_flow.size())
                        has_change = true;
                    else{
                        for(auto pair: cur_flow){
                            if(block->live_map.find(pair.first) == block->live_map.end()){
                                has_change = true;
                                break;
                            }

                            vector<FlowTuple*> &block_vec = block->live_map[pair.first];
                            vector<FlowTuple*> &flow_vec = pair.second;
                            if(block_vec.size() != flow_vec.size()){
                                has_change = true;
                                break;
                            }

                            for(auto var: flow_vec){
                                auto it = find(block_vec.begin(), block_vec.end(), var);
                                if(it == block_vec.end()){
                                    has_change = true;
                                    break;
                                }
                            }
                        } // for(auto pair: cur_flow){
                    } // else{
                } // if(!has_change){

                // assign the block's out
                block->live_map = cur_flow;
            } // for(FlowBasicBlock *block: blocks){
        }while(has_change);
    }

    void activeAnalyze(){
        mylog::debug << "Start " + func_entry->name + " 's active analyzing...";

        bool has_change;
        do{
            has_change = false;

            for(auto it=blocks.rbegin(); it!=blocks.rend(); it++){
                FlowBasicBlock *block = *it;

                vector<FlowBasicBlock*> front_blocks;
                vector<FlowBasicBlock*> back_blocks;
                for(FlowEdge *edge: block->in_edges)
                    front_blocks.push_back(edge->from);
                for(FlowEdge *edge: block->out_edges)
                    back_blocks.push_back(edge->to);

                // cal the block's out
                VarMap cur_flow;
                for(auto back_block: back_blocks)
                    mergeVarMap(cur_flow, back_block->active_map);

                // anlayze tuple by tuple
                for(auto tuple_it=block->tuples.rbegin(); tuple_it!=block->tuples.rend(); tuple_it++){
                    FlowTuple *tuple = *tuple_it;

                    // assign flow
                    tuple->active_map = cur_flow;

                    // update flow
                    for(const VarEntry *var: tuple->def()){
                        cur_flow[var].clear();
                        if(cur_flow[var].size() < 1)
                            cur_flow.erase(var);
                    }
                    for(const VarEntry *var: tuple->use()){
                        cur_flow[var].clear();
                        cur_flow[var].push_back(tuple);
                    }
                }

                // detect change
                if(!has_change){
                    if(block->active_map.size() != cur_flow.size())
                        has_change = true;
                    else{
                        for(auto pair: cur_flow){
                            if(block->active_map.find(pair.first) == block->active_map.end()){
                                has_change = true;
                                break;
                            }

                            vector<FlowTuple*> &block_vec = block->active_map[pair.first];
                            vector<FlowTuple*> &flow_vec = pair.second;
                            if(block_vec.size() != flow_vec.size()){
                                has_change = true;
                                break;
                            }

                            for(auto var: flow_vec){
                                auto it = find(block_vec.begin(), block_vec.end(), var);
                                if(it == block_vec.end()){
                                    has_change = true;
                                    break;
                                }
                            }
                        } // for(auto pair: cur_flow){
                    } // else{
                } // if(!has_change){

                // assign the block's in
                block->active_map = cur_flow;
            } // for(FlowBasicBlock *block: blocks){
        }while(has_change);
    }

    FlowBasicBlock *enter_block;
    vector<FlowBasicBlock*> exit_blocks;

    vector<FlowBasicBlock*> blocks;
    const FuncEntry *func_entry;
};

#endif//FLOW_FUNC_BLOCK_H