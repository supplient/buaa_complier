#ifndef FLOW_FUNC_BLOCK_H
#define FLOW_FUNC_BLOCK_H

#include "FlowBasicBlock.h"
#include "NameTableEntry.h"

#include "FuncBlock.h"

class FlowFuncBlock{
public:
    FlowFuncBlock(FuncBlock *func_block){
        // TODO
    }

    string toString(){
        // TODO
        return "";
    }

    FlowBasicBlock *enter_block;
    vector<FlowBasicBlock*> exit_blocks;

    vector<FlowBasicBlock*> blocks;
    const FuncEntry *func_entry;
};

#endif//FLOW_FUNC_BLOCK_H