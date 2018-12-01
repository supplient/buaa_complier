#ifndef FUNC_BACKEND_H
#define FUNC_BACKEND_H

#include <vector>
#include <stack>

#include "Command.h"
#include "FuncTuple.h"
#include "NameTable.h"
#include "RegPool.h"
#include "SizeUtil.h"

class FuncBackend
{
public:
    FuncBackend(NameTable &tab, const FuncTuple *func_tuple);

    void trans(map<string, string> &str_tab,
        vector<DataCmd*> *data_cmds, vector<InstCmd*> *inst_cmds);

    const FuncTuple *func_tuple;

private:
    RegPool reg_pool;

    // local var offset table
    // key: local var's full name (NameUtil::genEntryName)
    // value: offset between $sp
    map<string, back::size> lvo_tab;
    map<back::REG, stack<back::size> > param_stacks;

    back::size ra_offset;
    back::size stack_size;

    int param_count = 0;

    void writeBackVar(const VarEntry *entry, back::REG reg, vector<InstCmd*> *inst_cmds);
    void loadVar(const VarEntry *entry, back::REG res, vector<InstCmd*> *inst_cmds);

    back::REG askForTempReg(vector<InstCmd*> *inst_cmds);

    back::REG registAndLoadVar(const VarEntry *entry, vector<InstCmd*> *inst_cmds);
    back::REG registVar(const VarEntry *entry, vector<InstCmd*> *inst_cmds);

    bool reserveReg(back::REG reg){
        return reg_pool.reserveReg(reg);
    }
    void clearTempRegReserve(){
        reg_pool.clearTempRegReserve();
    }

    void saveTempReg(vector<InstCmd*> *inst_cmds);
    void restoreTempReg(vector<InstCmd*> *inst_cmds);

    void transTuple(Tuple *tuple, map<string, string> &str_tab,
        vector<DataCmd*> *data_cmds, vector<InstCmd*> *inst_cmds);
};

#endif//FUNC_BACKEND_H