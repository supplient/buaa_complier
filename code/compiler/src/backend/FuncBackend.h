#ifndef FUNC_BACKEND_H
#define FUNC_BACKEND_H

#include <vector>

#include "Command.h"
#include "FuncTuple.h"
#include "NameTable.h"
#include "RegPool.h"

class FuncBackend
{
public:
    FuncBackend(const FuncTuple *func_tuple)
        :func_tuple(func_tuple)
    {}

    void trans(NameTable &tab, map<string, string> str_tab,
        vector<DataCmd*> *data_cmds, vector<InstCmd*> *inst_cmds);

    const FuncTuple *func_tuple;
    RegPool reg_pool;
};

#endif//FUNC_BACKEND_H