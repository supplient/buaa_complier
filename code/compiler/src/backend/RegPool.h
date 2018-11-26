#ifndef REG_POOL_H
#define REG_POOL_H

#include "TempRegPool.h"

class RegPool
{
public:
    void resetTempReg(){
        temp_pool.reset();
    }

    back::REG regist(VarEntry *in_entry, VarEntry **out_entry){
        // TODO now is just TempRegPool 's wrapper
        return temp_pool.regist(in_entry, out_entry);
    }

    back::REG lookUpReg(VarEntry *entry){
        return temp_pool.lookUpReg(entry);
    }

private:
    TempRegPool temp_pool;
};

#endif//REG_POOL_H