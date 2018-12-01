#ifndef GLOBAL_REG_POOL_H
#define GLOBAL_REG_POOL_H

#include "NameTableEntry.h"
#include "Reg.h"

#include <map>

class GlobalRegPool
{
public:
    static bool contain(back::REG reg){
        if(reg >= back::s0 && reg < back::GLOBAL_REG_UP)
            return true;
        return false;
    }

public:
    GlobalRegPool(){
        for(unsigned int i=0; i<back::GLOBAL_REG_UP - back::s0; i++){
            reg_list.push_back(NULL);
        }
        ng = back::s0;
    }

    void reset(){
        for(unsigned int i=0; i<back::GLOBAL_REG_UP - back::s0; i++){
            reg_list[i] = NULL;
        }
        ng = back::s0;
    }

    back::REG regist(const VarEntry *in_entry){
        if(ng == back::GLOBAL_REG_UP)
            return back::NO_REG;

        back::REG res;

        // give the correponding reg
        res = ng;
        ng = ng + 1;
        reg_list[res-back::s0] = in_entry;

        return res;
    }

    back::REG lookUpReg(const VarEntry *entry){
        if(!entry)
            return back::NO_REG;

        for(unsigned int i=0; i<reg_list.size();i++){
            if(reg_list[i] == entry)
                return back::s0 + i;
        }

        return back::NO_REG;
    }

    const VarEntry *lookUpEntry(back::REG reg){
        return reg_list[reg - back::s0];
    }

    map<back::REG, const VarEntry*> getAllRegistedVar(){
        map<back::REG, const VarEntry*> res;
        for(unsigned int i=0; i<reg_list.size(); i++){
            if(!reg_list[i])
                continue;
            res[back::s0 + i] = reg_list[i];
        }
        return res;
    }

private:
    vector<const VarEntry*> reg_list;
    back::REG ng;

    const VarEntry* getRegEntry(back::REG reg){
        if(!GlobalRegPool::contain(reg))
            return NULL;
        return reg_list[reg-back::s0];
    }
};

#endif//GLOBAL_REG_POOL_H
