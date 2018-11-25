#include "Backend.h"

#include <map>
#include <string>

#include "NameTable.h"

using namespace std;


namespace back
{
    typedef unsigned int size; // count by byte
} // back

void Backend::trans(NameTable &tab, Tuples &tuples, 
        vector<DataCmd*> *data_cmds, vector<InstCmd*> *inst_cmds){

    // Declare string table, register pool, local var address table
    // TODO
    map<string, back::size> lv_adt; // local var address table

    // global vars' space decl DataCmd
    // TODO
    FuncNameTable *gtab = tab.getFuncNameTable(sem::GLOBAL_FUNC_NAME);
    if(!gtab)
        throw string("Backend: Cannot found global func name table.");
    vector<VarEntry*> gv_list = gtab->getUnconstVars();
    for(VarEntry* gv: gv_list){
    }

    // init local var address table
    // TODO

    //
}