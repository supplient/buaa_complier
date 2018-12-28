#ifndef GLOBAL_REG_POOL_H
#define GLOBAL_REG_POOL_H

#include "NameTableEntry.h"
#include "Reg.h"

#include <map>
#include <set>

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
    }

    void reset(){
        reg_map.clear();
        using_reg_map.clear();
    }

    back::REG regist(const VarEntry *in_entry, back::REG reg){
        back::REG res;

        if(!GlobalRegPool::contain(reg))
            return back::NO_REG;

        reg_map[reg].insert(in_entry);
        return reg;
    }

    void updateUsing(const VarEntry *entry, back::REG reg){
        if(!GlobalRegPool::contain(reg))
            throw string("GlobalRegPool: trying to update using_reg_map with reg " + to_string(reg) + " which is not a global reg.");
        using_reg_map[reg] = entry;
    }

    back::REG lookUpReg(const VarEntry *entry){
        if(!entry)
            return back::NO_REG;

        for(auto pair: reg_map){
            if(pair.second.find(entry) != pair.second.end())
                return pair.first;
        }

        return back::NO_REG;
    }

    const VarEntry* lookUpEntry(back::REG reg){
        if(!GlobalRegPool::contain(reg))
            throw string("GlobalRegPool: trying to look up entry with reg " + to_string(reg) + " which is not a global reg.");
        if(using_reg_map.find(reg) != using_reg_map.end())
            return using_reg_map[reg];
        else
            return NULL;
    }

    map<back::REG, const VarEntry*> getAllRegistedVar(){
        return using_reg_map;
    }

private:
    vector<const VarEntry*> reg_list;
    map<back::REG, set<const VarEntry*> > reg_map;
    map<back::REG, const VarEntry*> using_reg_map; // TODO maintain this

    const VarEntry* getRegEntry(back::REG reg){
        if(!GlobalRegPool::contain(reg))
            return NULL;
        return reg_list[reg-back::s0];
    }
};

#endif//GLOBAL_REG_POOL_H
