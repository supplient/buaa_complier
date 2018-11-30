#ifndef REG_POOL_H
#define REG_POOL_H

#include "TempRegPool.h"
#include "ParamRegPool.h"

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

    bool reserveReg(back::REG reg){
        if(temp_pool.contain(reg))
            return temp_pool.reserveReg(reg);
        throw string("Reserve for reg: " + to_string(reg) + " is not implemented.");
        return false;
    }

    void clearTempRegReserve(){
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

        back::REG res;

        /*
        if(in_entry->is_param){
            // is param, try to use param reg
            res = param_pool.regist(in_entry);
            if(res != back::NO_REG){
                // if param reg has free
                *out_entry = NULL;
                return res;
            }
            // if no param reg is free, regist a temp reg
        }
        */

        return temp_pool.regist(in_entry, out_entry);
    }

    back::REG unregist(const VarEntry *entry){
        auto it = speical_var_reg_pool.find(entry->name);
        if(it != speical_var_reg_pool.end())
            throw string("RegPool: cannot unregist speical var: " + entry->name);

        /*
        if(param_pool.lookUpReg(entry) != back::NO_REG)
            throw string("RegPool: cannot unregist a single param reg. Can only reset all param regs.");
        */

        return temp_pool.unregist(entry);
    }

    back::REG lookUpReg(const VarEntry *entry){
        // search special_var_reg_pool
        auto it = speical_var_reg_pool.find(entry->name);
        if(it != speical_var_reg_pool.end())
            return it->second;

        back::REG res;
        
        /*
        // search param pool
        res = param_pool.lookUpReg(entry);
        if(res != back::NO_REG)
            return res;
        */

        // search temp pool
        res = temp_pool.lookUpReg(entry);
        if(res != back::NO_REG)
            return res;

        // not found
        return back::NO_REG;
    }

    map<back::REG, const VarEntry*> getAllRegistedVar(){
        map<back::REG, const VarEntry*> res;

        /*
        auto param_res = param_pool.getAllRegistedVar();
        res.insert(param_res.begin(), param_res.end());
        */

        auto temp_res = temp_pool.getAllRegistedVar();
        res.insert(temp_res.begin(), temp_res.end());
        
        return res;
    }

    map<back::REG, const VarEntry*> getAllRegistedParamReg(){
        return param_pool.getAllRegistedVar();
    }

    map<back::REG, const VarEntry*> getAllRegistedTempReg(){
        return temp_pool.getAllRegistedVar();
    }

private:
    map<string, back::REG> speical_var_reg_pool;
    TempRegPool temp_pool;
    ParamRegPool param_pool;
};

#endif//REG_POOL_H