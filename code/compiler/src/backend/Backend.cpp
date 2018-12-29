#include "Backend.h"

#include <map>
#include <string>

#include "NameTable.h"
#include "Memory.h"

using namespace std;

void Backend::trans(NameTable &tab, GlobalRegAllocator *global_reg_allocator, const vector<FuncTuple*> &func_tuples, 
        vector<DataCmd*> *data_cmds, vector<InstCmd*> *inst_cmds){
        
    // check input
    if(!data_cmds)
        throw string("Backend.trans: must give a non-NULL data_cmds.");
    if(!inst_cmds)
        throw string("Backend.trans: must give a non-NULL inst_cmds.");

    // declare string table
    map<string, string> str_tab;

    // temp memory's space decl DataCmd
    DataCmd *tmp_cmd = new DataCmd();
    tmp_cmd->label = NameUtil::genGlobalVarLabel(back::TEMP_MEM_NAME);
    tmp_cmd->op = DataCmd::WORD;
    data_cmds->push_back(tmp_cmd);

    // global vars' space decl DataCmd
    FuncNameTable *gtab = tab.getFuncNameTable(sem::GLOBAL_FUNC_NAME);
    if(!gtab)
        throw string("Backend: Cannot found global func name table.");
    vector<VarEntry*> gv_list = gtab->getUnconstVars();
    for(VarEntry* gv: gv_list){
        if(!gv)
            throw string("Backend: NULL global varaible!");

        DataCmd *cmd = new DataCmd();
        if(gv->dim == 0){
            // normal var
            cmd->label = NameUtil::genGlobalVarLabel(gv->name);
            cmd->op = DataCmd::WORD;
        }
        else{
            // array var
            cmd->label = NameUtil::genGlobalVarLabel(gv->name);
            cmd->op = DataCmd::SPACE;
            cmd->size = 4*gv->dim;
        }
        data_cmds->push_back(cmd);
    }

    // build func backends
    vector<FuncBackend*> func_backends;
    for(FuncTuple *func_tuple: func_tuples)
        func_backends.push_back(new FuncBackend(tab, global_reg_allocator, func_tuple));

    // trans each function
    for(FuncBackend *func_backend: func_backends)
        func_backend->trans(str_tab, data_cmds, inst_cmds);

    // program concered trans
        // jump to program start label
    inst_cmds->insert(inst_cmds->begin(),
        new InstCmd(InstCmd::J, NameUtil::genFuncLabel(sem::GLOBAL_FUNC_NAME))
    );
        // mark program start label
    inst_cmds->push_back(
        new InstCmd(NameUtil::genFuncLabel(sem::GLOBAL_FUNC_NAME))
    );
        // call function
    inst_cmds->push_back(
        new InstCmd(InstCmd::JAL, NameUtil::genFuncLabel(sem::MAIN_FUNC_NAME))
    );
    inst_cmds->push_back(
        new InstCmd(InstCmd::NOP)
    );
}