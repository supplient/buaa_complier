#ifndef PARAM_REG_POOL_H
#define PARAM_REG_POOL_H

#include "NameTableEntry.h"
#include "Reg.h"

#include <map>

class ParamRegPool
{
public:
    static bool contain(back::REG reg){
        if(reg >= back::a0 && reg < back::PARAM_REG_UP)
            return true;
        return false;
    }

public:
    ParamRegPool(){
        for(unsigned int i=0; i<back::PARAM_REG_UP - back::a0; i++){
            assigned.push_back(false);
            reg_list.push_back(NULL);
        }
    }

    void reset(){
        for(unsigned int i=0; i<back::PARAM_REG_UP - back::a0; i++){
            assigned[i] = false;
            reg_list[i] = NULL;
        }
    }

    back::REG regist(int param_index, const VarEntry *in_entry=NULL){
        // check param_index
        if(param_index >= back::PARAM_REG_UP - back::a0)
            throw string("ParamRegPool: trying to regist too many param regs.");
        // check if is param
        if(in_entry){
            if(!in_entry->is_param)
                throw string("ParamRegPool: trying to regist a non-param var for the param reg.");
        }
        // check if assigned
        if(assigned[param_index])
            throw string("ParamRegPool: trying to regist an assigned param reg.");

        back::REG res;

        // give the correponding reg
        res = static_cast<back::REG>(param_index + back::a0);
        reg_list[res-back::a0] = in_entry;
        assigned[res-back::a0] = true;

        return res;
    }

    back::REG lookUpReg(const VarEntry *entry){
        if(!entry)
            return back::NO_REG;

        for(unsigned int i=0; i<reg_list.size();i++){
            if(reg_list[i] == entry)
                return static_cast<back::REG>(i+back::a0);
        }

        return back::NO_REG;
    }

    const VarEntry *lookUpEntry(back::REG reg){
        return reg_list[reg - back::a0];
    }

    bool checkIfAssigned(int param_index){
        // check param_index
        if(param_index >= back::PARAM_REG_UP - back::a0)
            throw string("ParamRegPool: trying to regist too many param regs.");
        return assigned[param_index];
    }

    map<back::REG, const VarEntry*> getAllRegistedVar(){
        map<back::REG, const VarEntry*> res;
        for(unsigned int i=0; i<reg_list.size(); i++){
            if(!assigned[i])
                continue;
            res[static_cast<back::REG>(i+back::a0)] = reg_list[i];
        }
        return res;
    }

private:
    vector<bool> assigned;
    vector<const VarEntry*> reg_list;

    const VarEntry* getRegEntry(back::REG reg){
        if(!ParamRegPool::contain(reg))
            return NULL;
        return reg_list[reg-back::a0];
    }
};

#endif//PARAM_REG_POOL_H