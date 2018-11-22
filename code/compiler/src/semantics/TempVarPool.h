#ifndef TEMP_VAR_POOL_H
#define TEMP_VAR_POOL_H

#include "NameUtil.h"
#include "NameTableEntry.h"
#include "NameTable.h"

class TempVarPool
{
public:
    TempVarPool(NameTable &tab, const string &func_name)
        :tab(tab), func_name(func_name), var_i(0)
    {}

    VarEntry* getNewIntTempVar(){
        return getNewTempVar(sym::INT);
    }

    VarEntry* getNewTempVar(sym::SYMBOL type){
        VarEntry *res = NULL;

        do{
            string var_name = NameUtil::genTempVarName(var_i);
            res = tab.lookUpVarOnlyCurrentFunc(func_name, var_name);
            if(!res)
                res = tab.insertVar(func_name, var_name, type, 0);
            var_i++;
        }while(res->type != type);

        return res;
    }

private:
    NameTable &tab;
    const string func_name;
    unsigned int var_i;
};

#endif//TEMP_VAR_POOL_H