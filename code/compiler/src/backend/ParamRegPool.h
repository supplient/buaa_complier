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
        for(unsigned int i=0; i<back::PARAM_REG_UP - back::a0; i++)
            reg_list.push_back(NULL);
    }

    void reset(){
        for(unsigned int i=0; i<back::PARAM_REG_UP - back::a0; i++)
            reg_list[i] = NULL;
    }

    back::REG regist(const VarEntry *in_entry, const VarEntry **out_entry){
        // check input
        if(!in_entry)
            throw string("TempRegPool: cannot reg a NULL entry on a temp reg.");
        // check if is param
        if(!in_entry->is_param)
            return back::NO_REG;

        back::REG res;

        // check if exists
        res = lookUpReg(in_entry);
        if(res != back::NO_REG){
            return res;
        }

        // check if param regs are enough for this param's index
        if(in_entry->param_index >= reg_list.size())
            return back::NO_REG;

        // give the correponding reg
        res = static_cast<back::REG>(in_entry->param_index + back::a0);
        *out_entry = getRegEntry(res);
        setEntryForReg(res, in_entry);

        return res;
    }

    void unregist(const VarEntry *entry){
        for(unsigned int i=0; i<reg_list.size(); i++){
            if(reg_list[i] == entry){
                reg_list[i] = NULL;
                return;
            }
        }
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

    map<back::REG, const VarEntry*> getAllRegistedVar(){
        map<back::REG, const VarEntry*> res;
        for(unsigned int i=0; i<reg_list.size(); i++){
            if(reg_list[i] == NULL)
                continue;
            res[static_cast<back::REG>(i+back::a0)] = reg_list[i];
        }
        return res;
    }

private:
    vector<const VarEntry*> reg_list;

    void setEntryForReg(back::REG reg, const VarEntry *entry){
        reg_list[reg-back::a0] = entry;
    }

    const VarEntry* getRegEntry(back::REG reg){
        if(!ParamRegPool::contain(reg))
            return NULL;
        return reg_list[reg-back::a0];
    }
};

#endif//PARAM_REG_POOL_H