#ifndef GLOBAL_REG_ALLOCATOR_H
#define GLOBAL_REG_ALLOCATOR_H

#include "NameTable.h"
#include "NameTableEntry.h"
#include "Reg.h"

class GlobalRegAllocator{
public:
    virtual vector<const VarEntry*> alloc(const FuncEntry* func_entry)=0;
};

class LinearGlobalRegAllocator:public GlobalRegAllocator{
public:
    LinearGlobalRegAllocator(NameTable &tab):tab(tab){}

    vector<const VarEntry*> alloc(const FuncEntry* func_entry){
        vector<const VarEntry*> res;

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
            
            res.push_back(lv);
            gr_count++;
        }

        return res;
    }

private:
    NameTable &tab;
};

#endif//GLOBAL_REG_ALLOCATOR_H