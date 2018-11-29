#ifndef REG_POOL_H
#define REG_POOL_H

#include "TempRegPool.h"

class RegPool
{
public:
    RegPool(){
        speical_var_reg_pool[NameUtil::getIntReturnVarName()] = back::v0;
        speical_var_reg_pool[NameUtil::getCharReturnVarName()] = back::v0;
    }

    void resetTempReg(){
        temp_pool.reset();
    }

    bool reserveReg(back::REG reg){
        if(temp_pool.contain(reg))
            return temp_pool.reserveReg(reg);
        throw string("Reserve for reg: " + to_string(reg) + " is not implemented.");
        return false;
    }

    void clearReserve(){
        temp_pool.clearReserve();
    }

    bool hasTargetMemory(back::REG reg){
        if(reg == back::v0)
            return false;
        return temp_pool.hasTargetMemory(reg);
    }

    back::REG askForTempReg(const VarEntry **out_entry){
        return temp_pool.askForTempReg(out_entry);
    }

    back::REG regist(const VarEntry *in_entry, const VarEntry **out_entry){
        if(lookUpReg(in_entry) != back::NO_REG){
            *out_entry = NULL;
            return lookUpReg(in_entry);
        }

        // TODO now is just TempRegPool 's wrapper
        return temp_pool.regist(in_entry, out_entry);
    }

    back::REG lookUpReg(const VarEntry *entry){
        auto it = speical_var_reg_pool.find(entry->name);
        if(it != speical_var_reg_pool.end())
            return it->second;

        return temp_pool.lookUpReg(entry);
    }

    back::REG unregist(const VarEntry *entry){
        auto it = speical_var_reg_pool.find(entry->name);
        if(it != speical_var_reg_pool.end())
            throw string("RegPool: cannot unregist speical var: " + entry->name);

        return temp_pool.unregist(entry);
    }

    map<back::REG, const VarEntry*> getAllRegistedVar(){
        return temp_pool.getAllRegistedVar();
    }

    map<back::REG, const VarEntry*> getAllRegistedTempReg(){
        return temp_pool.getAllRegistedVar();
    }

private:
    map<string, back::REG> speical_var_reg_pool;
    TempRegPool temp_pool;
};

#endif//REG_POOL_H