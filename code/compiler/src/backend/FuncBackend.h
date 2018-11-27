#ifndef FUNC_BACKEND_H
#define FUNC_BACKEND_H

#include <vector>

#include "Command.h"
#include "FuncTuple.h"
#include "NameTable.h"
#include "RegPool.h"
#include "SizeUtil.h"

class FuncBackend
{
public:
    FuncBackend(NameTable &tab, const FuncTuple *func_tuple);

    void trans(map<string, string> str_tab,
        vector<DataCmd*> *data_cmds, vector<InstCmd*> *inst_cmds);

    const FuncTuple *func_tuple;

private:
    RegPool reg_pool;

    // local var offset table
    // key: local var's full name (NameUtil::genEntryName)
    // value: offset between $sp
    map<string, back::size> lvo_tab;

    back::size ra_offset;
    back::size stack_size;

    void writeBackVar(const VarEntry *entry, back::REG reg, vector<InstCmd*> *inst_cmds);

    back::REG registAndLoadVar(const NameTableEntry *entry, vector<InstCmd*> *inst_cmds){
        return registAndLoadVar(dynamic_cast<const VarEntry*>(entry), inst_cmds);
    }
    back::REG registAndLoadVar(const VarEntry *entry, vector<InstCmd*> *inst_cmds);

    back::REG registVar(const NameTableEntry *entry, vector<InstCmd*> *inst_cmds){
        return registVar(dynamic_cast<const VarEntry*>(entry), inst_cmds);
    }
    back::REG registVar(const VarEntry *entry, vector<InstCmd*> *inst_cmds);

    void transTuple(Tuple *tuple, map<string, string> str_tab,
        vector<DataCmd*> *data_cmds, vector<InstCmd*> *inst_cmds);
};

#endif//FUNC_BACKEND_H