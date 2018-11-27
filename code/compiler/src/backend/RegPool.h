#ifndef REG_POOL_H
#define REG_POOL_H

#include "TempRegPool.h"

class RegPool
{
public:
    void resetTempReg(){
        temp_pool.reset();
    }

    back::REG regist(const VarEntry *in_entry, const VarEntry **out_entry){
        // TODO now is just TempRegPool 's wrapper
        return temp_pool.regist(in_entry, out_entry);
    }

    back::REG lookUpReg(const VarEntry *entry){
        return temp_pool.lookUpReg(entry);
    }

    back::REG unregist(const VarEntry *entry){
        return temp_pool.unregist(entry);
    }

    map<back::REG, const VarEntry*> getAllRegistedVar(){
        return temp_pool.getAllRegistedVar();
    }

private:
    TempRegPool temp_pool;
};

#endif//REG_POOL_H