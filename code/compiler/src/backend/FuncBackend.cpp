#include "FuncBackend.h"

#include <algorithm>

#include "symbol.h"
#include "log.h"

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

    // DEBUG
    mylog::debug << "Func " + func_tuple->func_entry->name + " 's lvo_tab.";
    for(auto pair: lvo_tab){
        mylog::debug << pair.first + ": " + to_string(pair.second);
    }
}

bool isGlobalVar(VarEntry *entry){
    return entry->getOwnerName() == sem::GLOBAL_FUNC_NAME;
}

back::REG FuncBackend::loadVar(VarEntry *entry, vector<InstCmd*> *inst_cmds){
    back::REG res;

    // check if has registed
    res = reg_pool.lookUpReg(entry);
    if(res != back::NO_REG)
        return res;

    // ask for a reg
    VarEntry *out_entry = NULL;
    res = reg_pool.regist(entry, &out_entry);

    if(out_entry){
        // some var has been unregisted
        // write back out_entry
        InstCmd *write_cmd = new InstCmd();
        switch(out_entry->type){
            case sym::INT:
                write_cmd->op = InstCmd::SW;
                break;
            case sym::CHAR:
                write_cmd->op = InstCmd::SB;
                break;
            default:
                throw string("FuncBackend.loadVar: invalid out_entry's type: " + to_string(out_entry->type));
        }
        write_cmd->res_reg = res;
        if(isGlobalVar(out_entry)){
            write_cmd->left_reg = back::NO_REG;
            write_cmd->right_type = InstCmd::LABEL_TYPE;
            write_cmd->right_label = NameUtil::genEntryName(out_entry);
        }
        else{
            write_cmd->left_reg = back::sp;
            write_cmd->right_type = InstCmd::IMME_TYPE;
            write_cmd->right_imme = lvo_tab[NameUtil::genEntryName(out_entry)];
        }

        inst_cmds->push_back(write_cmd);
    }

    // load var's value into the registed reg.
    InstCmd *read_cmd = new InstCmd();
    switch(entry->type){
        case sym::INT:
            read_cmd->op = InstCmd::LW;
            break;
        case sym::CHAR:
            read_cmd->op = InstCmd::LB;
            break;
        default:
            throw string("FuncBackend.loadVar: invalid entry's type: " + to_string(entry->type));
    }
    read_cmd->res_reg = res;
    if(isGlobalVar(entry)){
        read_cmd->left_reg = back::NO_REG;
        read_cmd->right_type = InstCmd::LABEL_TYPE;
        read_cmd->right_label = NameUtil::genEntryName(entry);
    }
    else{
        read_cmd->left_reg = back::sp;
        read_cmd->right_type = InstCmd::IMME_TYPE;
        read_cmd->right_imme = lvo_tab[NameUtil::genEntryName(entry)];
    }

    inst_cmds->push_back(read_cmd);

    return res;
}

void FuncBackend::trans(map<string, string> str_tab,
    vector<DataCmd*> *data_cmds, vector<InstCmd*> *inst_cmds){

    using namespace sem;

    for(Tuple *tuple: func_tuple->tuples){
    }
}
