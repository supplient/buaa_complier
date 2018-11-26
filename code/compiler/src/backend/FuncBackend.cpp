#include "FuncBackend.h"

#include <algorithm>

FuncBackend::FuncBackend(NameTable &tab, const FuncTuple *func_tuple)
    :func_tuple(func_tuple)
{
    back::size offset = 0;

    // global regs
    // TODO

    // $ra
    ra_offset = offset;
    offset += SizeUtil::regSize();

    // temp var & local var
    // we treat temp var as local var
    FuncNameTable *func_tab = tab.getFuncNameTable(func_tuple->func_entry->name);
    vector<VarEntry*> lv_list = func_tab->getUnconstVars();
    for(VarEntry *lv: lv_list){
        if(!lv)
            throw string("FuncBackend: NULL local variable!");
        
        if(lv->is_param)
            continue; // param is not processed here.

        lvo_tab[NameUtil::genEntryName(lv)] = offset;

        // Note: we save char in one word, just the same as int
        if(lv->dim == 0){
            // normal var
            offset += SizeUtil::regSize();
        }
        else{
            // array var
            offset += SizeUtil::regSize() * lv->dim;
        }
    }

    // Save stack size
    stack_size = offset;

    // param
    vector<VarEntry*> param_list = func_tuple->func_entry->param_list;
    for(VarEntry *pv: param_list){
        if(!pv)
            throw string("FuncBackend: NULL param!");

        lvo_tab[NameUtil::genEntryName(pv)] = offset;
        offset += SizeUtil::regSize(); // no param can be an array.
    }
}

void FuncBackend::trans(map<string, string> str_tab,
    vector<DataCmd*> *data_cmds, vector<InstCmd*> *inst_cmds){

    
    using namespace sem;
    
    for(Tuple *tuple: func_tuple->tuples){
    }
}
