#ifndef DAG_VAR_POOL_H
#define DAG_VAR_POOL_H

#include "NameTable.h"
#include "NameUtil.h"

namespace dag
{

    class VarPool
    {
    public:
        VarPool(FuncNameTable *func_tab)
            :func_tab(func_tab), var_i(0)
        {}

        VarEntry* getNewTempVar(sym::SYMBOL type){
            VarEntry *res = NULL;

            do{
                string var_name = NameUtil::genDAGVarName(var_i);
                NameTableEntry *entry = func_tab->lookUp(var_name);
                res = dynamic_cast<VarEntry*>(entry);
                if(!res && entry)
                    throw string("dag::VarPool.getNewTempVar: the var [" + var_name + "] is in tab, but cannot be converted to be a VarEntry.");
                if(!res)
                    res = func_tab->insertVar(var_name, type, 0);
                var_i++;
            }while(res->type != type);

            return res;
        }

    private:
        FuncNameTable *func_tab;
        unsigned int var_i;
    };

} // dag

#endif//DAG_VAR_POOL_H