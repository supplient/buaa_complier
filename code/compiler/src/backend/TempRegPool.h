#ifndef TEMP_REG_POOL_H
#define TEMP_REG_POOL_H

#include <algorithm>

#include "NameTableEntry.h"
#include "Reg.h"

class TempRegPool
{
public:
    static bool contain(back::REG reg){
        if(reg >= back::t0 && reg < back::TEMP_REG_UP)
            return true;
        return false;
    }
public:
    TempRegPool(){
        // fill reg_list with NULL
        for(unsigned int i=0; i<back::TEMP_REG_UP - back::t0; i++)
            reg_list.push_back(NULL);

        // init index
        ng = back::t0;
    }

    void reset(){
        for(unsigned int i=0; i<reg_list.size(); i++)
            reg_list[i] = NULL;
        ng = back::t0;
        clearReserve();
    }

    bool reserveReg(back::REG reg){
        if(isReserved(reg)) // it has been reserved
            return false;
        reserved_reg.push_back(reg);
        return true;
    }

    void clearReserve(){
        reserved_reg.clear();
    }

    bool hasTargetMemory(back::REG reg){
        return reg_list[reg - back::t0] != NULL;
    }

    back::REG askForTempReg(const VarEntry **out_entry){
        // check input
        if(!out_entry)
            throw string("TempRegPool: must assign a out_entry when registering a temp reg.");

        // search for a free reg
        back::REG res = searchForReg(out_entry);

        // Note: No registing! Just skip.
        setEntryForReg(res, NULL);

        return res;
    }

    back::REG regist(const VarEntry *in_entry, const VarEntry **out_entry){
        // check input
        if(!out_entry)
            throw string("TempRegPool: must assign a out_entry when registering a temp reg.");
        if(!in_entry)
            throw string("TempRegPool: cannot reg a NULL entry on a temp reg.");

        back::REG res;

        // check if exists
        res = lookUpReg(in_entry);
        if(res != back::NO_REG){
            *out_entry = NULL;
            return res;
        }

        // search for a reg
        res = searchForReg(out_entry);

        // regist new reg
        setEntryForReg(res, in_entry);

        return res;
    }

    back::REG unregist(const VarEntry *entry){
        if(!entry)
            throw string("TempRegPool.unregist: Received a NULL entry.");

        back::REG res = lookUpReg(entry);
        if(res == back::NO_REG)
            return res;

        reg_list[res-back::t0] = NULL;
        return res;
    }

    back::REG lookUpReg(const VarEntry *entry){
        if(!entry)
            return back::NO_REG;

        for(unsigned int i=0; i<reg_list.size();i++){
            if(reg_list[i] == entry)
                return static_cast<back::REG>(i+back::t0);
        }

        return back::NO_REG;
    }

    const VarEntry* lookUpEntry(back::REG reg){
        return reg_list[reg - back::t0];
    }

    map<back::REG, const VarEntry*> getAllRegistedVar(){
        map<back::REG, const VarEntry*> res;
        for(unsigned int i=0; i<reg_list.size(); i++){
            if(reg_list[i] == NULL)
                continue;
            res[static_cast<back::REG>(i+back::t0)] = reg_list[i];
        }
        return res;
    }

private:
    back::REG ng;
    vector<const VarEntry*> reg_list;
    vector<back::REG> reserved_reg;

    void setEntryForReg(back::REG reg, const VarEntry *entry){
        reg_list[reg-back::t0] = entry;
    }

    void setNowRegEntry(const VarEntry *entry){
        setEntryForReg(ng, entry);
    }

    back::REG nowReg(){
        return ng;
    }

    const VarEntry* nowRegEntry(){
        return reg_list[ng-back::t0];
    }

    const VarEntry* nextRegEntry(){
        ng = static_cast<back::REG>((int)ng + 1);
        if(ng>=back::TEMP_REG_UP) // loop
            ng = back::t0;
        return reg_list[ng-back::t0];
    }

    bool isReserved(back::REG reg){
        auto it = find(reserved_reg.begin(), reserved_reg.end(), reg);
        return it != reserved_reg.end();
    }

    back::REG searchForReg(const VarEntry **out_entry){
        back::REG res;
        unsigned int try_times = 0;

        do{
            if(!nowRegEntry()){
                // nowReg is free
                // no need to free now reg
                *out_entry = NULL;
            }
            else{
                // must be no reg is free.
                // free now reg
                *out_entry = nowRegEntry();
            }
            res = nowReg();
            try_times++;
            nextRegEntry();
        }while(isReserved(res) && try_times<reg_list.size()+1);

        if(try_times == reg_list.size() + 1)
            throw string("TempRegPool: Cannot find any unreserved reg to use!");

        return res;
    }
};

#endif//TEMP_REG_POOL_H
