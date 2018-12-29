#include "FuncBackend.h"

#include <algorithm>

#include "symbol.h"
#include "log.h"
#include "config.h"
#include "Memory.h"

string reserve_fail = "Reserve fail.";

FuncBackend::FuncBackend(NameTable &tab, GlobalRegAllocator *global_reg_allocator, const FuncTuple *func_tuple)
    :func_tuple(func_tuple)
{
    vector<VarEntry*> param_list = func_tuple->func_entry->param_list;
    FuncNameTable *func_tab = tab.getFuncNameTable(func_tuple->func_entry->name);
    vector<VarEntry*> lv_list = func_tab->getUnconstVars();

    // Init param reg alloc, caller should promise for this
    int param_index = 0;
    for(VarEntry *entry: param_list){
        if(param_index >= back::PARAM_REG_UP - back::a0)
            break;
        reg_pool.registForParamReg(param_index, entry);
        param_index++;
    }


    // Init global reg alloc
    gr_count = 0;
    map<back::REG, vector<const VarEntry*> > global_alloc_res = global_reg_allocator->alloc(func_tuple->func_entry);
    for(auto pair: global_alloc_res){
        gr_count++;
        for(const VarEntry *entry: pair.second)
            reg_pool.registForGlobalReg(entry, pair.first);
    }


    // Construct stack
    back::size offset = 0;

    //  global regs
    offset += SizeUtil::regSize() * gr_count;

    //  $ra
    ra_offset = offset;
    offset += SizeUtil::regSize();

    //  temp var & local var
    //  we treat temp var as local var
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

    //  save stack size
    stack_size = offset;

    //  param
    //  Note: since stack is built in the reversed order,
    //      the param is saved reversedly in memory.
    vector<VarEntry*> reversed_param_list = param_list;
    reverse(reversed_param_list.begin(), reversed_param_list.end());
    for(VarEntry *pv: reversed_param_list){
        if(!pv)
            throw string("FuncBackend: NULL param!");

        lvo_tab[NameUtil::genEntryName(pv)] = offset;
        offset += SizeUtil::regSize(); // no param can be an array.
    }

    // DEBUG
    mylog::debug << "Func " + func_tuple->func_entry->name + " 's lvo_tab.";
    for(auto pair: lvo_tab){
        mylog::debug << pair.first + ": " + to_string(pair.second);
    }
}

void FuncBackend::writeToTempMem(back::REG reg, vector<InstCmd*> *inst_cmds){
    InstCmd *write_cmd = new InstCmd();
    write_cmd->op = InstCmd::SW;
    write_cmd->res_reg = reg;
    write_cmd->left_reg = back::NO_REG;
    write_cmd->right_type = InstCmd::LABEL_TYPE;
    write_cmd->right_label = NameUtil::genGlobalVarLabel(back::TEMP_MEM_NAME);
    inst_cmds->push_back(write_cmd);
}

void FuncBackend::loadFromTempMem(back::REG reg, vector<InstCmd*> *inst_cmds){
    inst_cmds->push_back(
        new InstCmd(
            InstCmd::LW,
            reg,
            back::NO_REG,
            NameUtil::genGlobalVarLabel(back::TEMP_MEM_NAME)
        )
    );
}

void FuncBackend::writeBackVar(const VarEntry *entry, back::REG reg, vector<InstCmd*> *inst_cmds){
    InstCmd *write_cmd = new InstCmd();
    switch(entry->type){
        case sym::INT:
            write_cmd->op = InstCmd::SW;
            break;
        case sym::CHAR:
            write_cmd->op = InstCmd::SB;
            break;
        default:
            throw string("FuncBackend.writeBackVar: invalid entry's type: " + to_string(entry->type));
    }
    write_cmd->res_reg = reg;
    if(VarEntry::isGlobalVar(entry)){
        write_cmd->left_reg = back::NO_REG;
        write_cmd->right_type = InstCmd::LABEL_TYPE;
        write_cmd->right_label = NameUtil::genGlobalVarLabel(entry->name);
    }
    else{
        write_cmd->left_reg = back::sp;
        write_cmd->right_type = InstCmd::IMME_TYPE;
        write_cmd->right_imme = lvo_tab[NameUtil::genEntryName(entry)] + 4*param_count;
    }

    inst_cmds->push_back(write_cmd);
}

void FuncBackend::loadVar(const VarEntry *entry, back::REG reg, vector<InstCmd*> *inst_cmds){
    InstCmd *read_cmd = new InstCmd();
    switch(entry->type){
        case sym::INT:
            read_cmd->op = InstCmd::LW;
            break;
        case sym::CHAR:
            read_cmd->op = InstCmd::LB;
            break;
        default:
            throw string("FuncBackend.registAndLoadVar: invalid entry's type: " + to_string(entry->type));
    }
    read_cmd->res_reg = reg;
    if(VarEntry::isGlobalVar(entry)){
        read_cmd->left_reg = back::NO_REG;
        read_cmd->right_type = InstCmd::LABEL_TYPE;
        read_cmd->right_label = NameUtil::genGlobalVarLabel(entry->name);
    }
    else{
        read_cmd->left_reg = back::sp;
        read_cmd->right_type = InstCmd::IMME_TYPE;
        read_cmd->right_imme = lvo_tab[NameUtil::genEntryName(entry)] + 4*param_count;
    }

    inst_cmds->push_back(read_cmd);
}

back::REG FuncBackend::askForTempReg(vector<InstCmd*> *inst_cmds){
    const VarEntry *out_entry = NULL;

    back::REG res = reg_pool.askForTempReg(&out_entry);

    if(out_entry){
        writeBackVar(out_entry, res, inst_cmds);
    }

    if(!reserveReg(res))
        throw reserve_fail;
    return res;
}

back::REG FuncBackend::registVar(const VarEntry *entry, vector<InstCmd*>* inst_cmds){
    back::REG res;

    // check if has registed
    res = reg_pool.lookUpReg(entry);
    if(res != back::NO_REG)
        return res;

    // ask for a reg
    const VarEntry *out_entry = NULL;
    res = reg_pool.regist(entry, &out_entry);

    if(out_entry){
        // some var has been unregisted
        // write back out_entry
        writeBackVar(out_entry, res, inst_cmds);
    }

    // reserve the reg
    if(!reserveReg(res))
        throw reserve_fail;
    return res;
}

back::REG FuncBackend::registAndLoadVar(const VarEntry *entry, vector<InstCmd*> *inst_cmds){
    back::REG res;

    // check if has registed
    res = reg_pool.lookUpReg(entry);
    if(res != back::NO_REG)
        return res;

    // regist if not registed
    res = registVar(entry, inst_cmds);

    // load var's value into the registed reg.
    loadVar(entry, res, inst_cmds);

    return res;
}

void FuncBackend::saveTempReg(vector<InstCmd*> *inst_cmds){
    map<back::REG, const VarEntry*> reg_entry_map = reg_pool.getAllRegistedTempReg();
    for(auto pair: reg_entry_map){
        writeBackVar(pair.second, pair.first, inst_cmds);
    }
}

void FuncBackend::restoreTempReg(vector<InstCmd*> *inst_cmds){
    map<back::REG, const VarEntry*> reg_entry_map = reg_pool.getAllRegistedTempReg();
    for(auto pair: reg_entry_map){
        loadVar(pair.second, pair.first, inst_cmds);
    }
}

void FuncBackend::saveAndResetTempReg(vector<InstCmd*> *inst_cmds){
    saveTempReg(inst_cmds);
    reg_pool.resetTempReg();
}

void FuncBackend::trans(map<string, string> &str_tab,
    vector<DataCmd*> *data_cmds, vector<InstCmd*> *inst_cmds){

    param_count = 0;
    for(Tuple *tuple: func_tuple->tuples){
        if(MIPS_TUPLE_OUTPUT){
            inst_cmds->push_back(
                new CommentCmd("#" + tuple->toString())
            );
        }
        transTuple(tuple, str_tab, data_cmds, inst_cmds);
        if(MIPS_TUPLE_OUTPUT){
            inst_cmds->push_back(
                new CommentCmd("")
            );
        }
    }
}