#ifndef TEMP_REG_POOL_H
#define TEMP_REG_POOL_H

#include "NameTableEntry.h"
#include "Reg.h"

class TempRegPool
{
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
    }

    back::REG askForTempReg(const VarEntry **out_entry){
        // check input
        if(!out_entry)
            throw string("TempRegPool: must assign a out_entry when registering a temp reg.");
        back::REG res;

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

        // Note: No registing! Just skip.
        setNowRegEntry(NULL);
        res = nowReg();
        nextRegEntry();

        return res;
    }

    back::REG regist(const VarEntry *in_entry, const VarEntry **out_entry){
        // check input
        if(!out_entry)
            throw string("TempRegPool: must assign a out_entry when registering a temp reg.");
        if(!in_entry)
            throw string("TempRegPool: cannot reg a NULL entry on a temp reg.");

        // check if exists
        if(lookUpReg(in_entry) != back::NO_REG){
            *out_entry = NULL;
            return lookUpReg(in_entry);
        }

        back::REG res;

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

        // regist new reg
        setNowRegEntry(in_entry);
        res = nowReg();
        nextRegEntry();

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

    back::REG nowReg(){
        return ng;
    }

    const VarEntry* nowRegEntry(){
        return reg_list[ng-back::t0];
    }

    void setNowRegEntry(const VarEntry *entry){
        reg_list[ng-back::t0] = entry;
    }

    const VarEntry* nextRegEntry(){
        ng = static_cast<back::REG>((int)ng + 1);
        if(ng>=back::TEMP_REG_UP) // loop
            ng = back::t0;
        return reg_list[ng-back::t0];
    }
};

#endif//TEMP_REG_POOL_H
