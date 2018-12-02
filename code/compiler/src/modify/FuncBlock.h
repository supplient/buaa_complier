#ifndef FUNC_BLOCK_H
#define FUNC_BLOCK_H

#include "BasicBlock.h"

class FuncBlock
{
public:
    FuncBlock(const FuncEntry *func_entry)
        :func_entry(func_entry)
    {
        if(!func_entry)
            throw string("FuncBlock: func_entry is NULL.");
    }

    string toString(){
        string s = "[Func Block " + func_entry->name + "]\n";

        s += "[ENTER\t" + to_string(enter_block->index) + "]\n";

        s += "[EXIT";
        for(BasicBlock *block: exit_blocks){
            s += "  " + to_string(block->index);
        }
        s += "]\n";

        s += "\n";

        for(BasicBlock *block: blocks){
            s += block->toString("\t") + "\n";
        }

        return s;
    }

    BasicBlock *enter_block;
    vector<BasicBlock*> exit_blocks;

    vector<BasicBlock*> blocks;
    const FuncEntry *func_entry;
};

#endif//FUNC_BLOCK_H