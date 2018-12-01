#ifndef REG_POOL_H
#define REG_POOL_H

#include "TempRegPool.h"
#include "ParamRegPool.h"
#include "GlobalRegPool.h"

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

    void resetParamReg(){
        param_pool.reset();
    }

    void resetGlobalReg(){
        global_pool.reset();
    }

    bool reserveReg(back::REG reg){
        if(temp_pool.contain(reg))
            return temp_pool.reserveReg(reg);
        throw string("Reserve for reg: " + to_string(reg) + " is not implemented.");
        return false;
    }

    void clearTempRegReserve(){
        temp_pool.clearReserve();
    }

    bool isParamRegFree(int param_index){
        return !param_pool.checkIfAssigned(param_index);
    }

    back::REG askForTempReg(const VarEntry **out_entry){
        return temp_pool.askForTempReg(out_entry);
    }

    back::REG registForParamReg(int param_index, const VarEntry *entry=NULL){
        // Note: We allow the entry to be NULL
        return param_pool.regist(param_index, entry);
    }

    back::REG registForGlobalReg(const VarEntry *entry){
        return global_pool.regist(entry);
    }

    // can only regist for temp reg
    //      param reg and global reg can only be registed by specifing the target register.
    back::REG regist(const VarEntry *in_entry, const VarEntry **out_entry){
        if(lookUpReg(in_entry) != back::NO_REG){
            *out_entry = NULL;
            return lookUpReg(in_entry);
        }

        return temp_pool.regist(in_entry, out_entry);
    }

    back::REG unregistTempReg(const VarEntry *entry){
        return temp_pool.unregist(entry);
    }

    back::REG lookUpReg(const VarEntry *entry){
        // search special_var_reg_pool
        auto it = speical_var_reg_pool.find(entry->name);
        if(it != speical_var_reg_pool.end())
            return it->second;

        back::REG res;
        
        // search param pool
        res = param_pool.lookUpReg(entry);
        if(res != back::NO_REG)
            return res;

        // search global pool
        res = global_pool.lookUpReg(entry);
        if(res != back::NO_REG)
            return res;

        // search temp pool
        res = temp_pool.lookUpReg(entry);
        if(res != back::NO_REG)
            return res;

        // not found
        return back::NO_REG;
    }

    const VarEntry *lookUpEntry(back::REG reg){
        if(reg == back::v0)
            throw string("RegPool.lookUpEntry: trying to look up $v0 's entry.");
        
        if(ParamRegPool::contain(reg))
            return param_pool.lookUpEntry(reg);

        if(GlobalRegPool::contain(reg))
            return global_pool.lookUpEntry(reg);

        if(TempRegPool::contain(reg))
            return temp_pool.lookUpEntry(reg);

        return NULL;
    }

    map<back::REG, const VarEntry*> getAllRegistedVar(){
        map<back::REG, const VarEntry*> res;

        auto param_res = param_pool.getAllRegistedVar();
        res.insert(param_res.begin(), param_res.end());

        auto global_res = global_pool.getAllRegistedVar();
        res.insert(global_res.begin(), global_res.end());

        auto temp_res = temp_pool.getAllRegistedVar();
        res.insert(temp_res.begin(), temp_res.end());
        
        return res;
    }

    map<back::REG, const VarEntry*> getAllRegistedParamReg(){
        return param_pool.getAllRegistedVar();
    }

    map<back::REG, const VarEntry*> getAllRegistedGlobalReg(){
        return global_pool.getAllRegistedVar();
    }

    map<back::REG, const VarEntry*> getAllRegistedTempReg(){
        return temp_pool.getAllRegistedVar();
    }

private:
    map<string, back::REG> speical_var_reg_pool;
    GlobalRegPool global_pool;
    ParamRegPool param_pool;
    TempRegPool temp_pool;
};

#endif//REG_POOL_H