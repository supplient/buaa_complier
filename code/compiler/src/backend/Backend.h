#ifndef BACKEND_H
#define BACKEND_H

#include <vector>

#include "Command.h"
#include "FuncTuple.h"
#include "NameTable.h"
#include "FuncBackend.h"

using namespace std;

class Backend // Actually MIPS Backend
{
public:
    void trans(NameTable &tab, const vector<FuncTuple*> &func_tuples, 
        vector<DataCmd*> *data_cmds, vector<InstCmd*> *inst_cmds);
};

#endif//BACKEND_H