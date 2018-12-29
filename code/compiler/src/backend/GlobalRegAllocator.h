#ifndef GLOBAL_REG_ALLOCATOR_H
#define GLOBAL_REG_ALLOCATOR_H

#include "NameTable.h"
#include "NameTableEntry.h"
#include "Reg.h"
#include "ConflictGraph.h"

class GlobalRegAllocator{
public:
    virtual map<back::REG, vector<const VarEntry*> > alloc(const FuncEntry* func_entry)=0;

    virtual string name()=0;
};


class LinearGlobalRegAllocator:public GlobalRegAllocator{
public:
    LinearGlobalRegAllocator(NameTable &tab):tab(tab){}

    map<back::REG, vector<const VarEntry*> > alloc(const FuncEntry* func_entry){
        map<back::REG, vector<const VarEntry*> > res;

        FuncNameTable *func_tab = tab.getFuncNameTable(func_entry->name);
        vector<VarEntry*> lv_list = func_tab->getUnconstVars();

        unsigned int gr_count = 0;
        for(VarEntry *lv: lv_list){
            if(gr_count >= back::GLOBAL_REG_UP - back::s0)
                break;
            if(!lv)
                throw string("LinearGlobalRegAllocator: NULL local variable in func <" + func_entry->name + ">!");
            if(lv->is_param)
                continue;// param cannot be allocated for global regs
            
            res[gr_count + back::s0].push_back(lv);
            gr_count++;
        }

        return res;
    }

    string name(){
        return "Linear Global Reg Allocator";
    }

private:
    NameTable &tab;
};


class GraphGlobalRegAllocator:public GlobalRegAllocator{
public:
    GraphGlobalRegAllocator(vector<FlowFuncBlock*> func_blocks){
        mylog::debug << "Start building conlict graphs...";
        for(FlowFuncBlock *func_block: func_blocks){
            string name = func_block->func_entry->name;
            if(conflict_graphs.count(name) != 0)
                throw string("GraphGlobalRegAllocator: Func <" + name + "> 's conflict graph has been built.");

            conflict_graphs[name] = new ConflictGraph(func_block);

            mylog::debug << "Func <" + name + ">--";
            mylog::debug << conflict_graphs[name]->toString() + "\n";
        }
    }

    // Warning!!! 
    //      For each func_entry, this function can only be called once, because conflict graph will be broken after one call.
    map<back::REG, vector<const VarEntry*> > alloc(const FuncEntry *func_entry){
        map<back::REG, vector<const VarEntry*> > res;

        ConflictGraph *graph = conflict_graphs[func_entry->name];
        unsigned int reg_num = back::GLOBAL_REG_UP - back::s0;
        back::REG reg = back::s0;

        while(graph->varNum()){
            // try to find a var conflict with less than reg_num
            const VarEntry *candi = graph->searchVarConflictLess(reg_num);

            if(candi){
                // found, alloc reg for it
                res[reg].push_back(candi);
                reg++;
                if(reg == back::GLOBAL_REG_UP)
                    reg = back::s0;
            }
            else{
                // not found, choose a var to give up
                candi = graph->giveVarToGiveUp();
                if(!candi)
                    throw string("GraphGlobalRegAllocator: cannot find a var to give up.");
            }

            // remove the selected var
            graph->removeVar(candi);
        }

        return res;
    }

    string name(){
        return "Graph Global Reg Allocator";
    }

private:
    map<string, ConflictGraph*> conflict_graphs;
};

#endif//GLOBAL_REG_ALLOCATOR_H