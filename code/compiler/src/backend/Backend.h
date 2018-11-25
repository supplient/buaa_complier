#ifndef BACKEND_H
#define BACKEND_H

#include <vector>

#include "Command.h"
#include "Tuple.h"
#include "NameTable.h"

using namespace std;

class Backend // Actually MIPS Backend
{
public:
    void trans(NameTable &tab, Tuples &tuples, 
        vector<DataCmd*> *data_cmds, vector<InstCmd*> *inst_cmds);
};

#endif//BACKEND_H