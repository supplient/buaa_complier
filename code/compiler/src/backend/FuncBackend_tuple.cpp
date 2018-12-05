#include "FuncBackend.h"

#include <algorithm>

#include "symbol.h"
#include "log.h"

bool isGlobalVar(const VarEntry *entry);

void FuncBackend::transTuple(Tuple *tuple, map<string, string> &str_tab,
    vector<DataCmd*> *data_cmds, vector<InstCmd*> *inst_cmds){
    using namespace sem;

    int index;
    int param_number;
    back::size offset;
    map<back::REG, const VarEntry*> reg_entry_map;
    const VarEntry *var_entry;
    InstCmd::OP inst_op;
    int sys_num;
    string str_label;
    back::REG res_reg;
    back::REG left_reg;
    back::REG right_reg;
    back::REG sel_reg;
    switch(tuple->op){
        case EMPTY:
            break;
        // EMPTY

        case LABEL:
            inst_cmds->push_back(
                new InstCmd(tuple->res->str_value)
            );
            break;
        // LABEL

        case ASSIGN:
            res_reg = registVar(tuple->res->entry, inst_cmds);

            if(tuple->left->type == Operand::ENTRY){
                // move
                left_reg = registAndLoadVar(tuple->left->entry, inst_cmds);
                inst_cmds->push_back(
                    new InstCmd(InstCmd::MOVE, res_reg, left_reg)
                );
            }
            else if(tuple->left->type == Operand::INT_CONST){
                // add
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, res_reg, back::zero, tuple->left->int_const)
                );
            }
            else if(tuple->left->type == Operand::CHAR_CONST){
                // add
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, res_reg, back::zero, tuple->left->char_const)
                );
            }
            else
                throw string("FuncBackend: Invalid tuple->right's type: " + to_string(tuple->right->type));
            // type conv
            if(tuple->res->entry->type == sym::CHAR){
                inst_cmds->push_back(
                    new InstCmd(InstCmd::AND, res_reg, res_reg, 0xff)
                );
            }
            break;
        // ASSIGN

        case NEG:
            // regist res
            res_reg = registVar(tuple->res->entry, inst_cmds);
            // load left
            if(tuple->left->type == Operand::ENTRY){
                var_entry = dynamic_cast<const VarEntry*>(tuple->left->entry);
                left_reg = registAndLoadVar(var_entry, inst_cmds);
                inst_cmds->push_back(
                    new InstCmd(InstCmd::NEG, res_reg, left_reg)
                );
            }
            else if(tuple->left->type == Operand::INT_CONST){
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, res_reg, back::zero, -tuple->left->int_const)
                );
            }
            else if(tuple->left->type == Operand::CHAR_CONST){
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, res_reg, back::zero, -tuple->left->char_const)
                );
            }
            // type conv
            if(tuple->res->entry->type == sym::CHAR){
                inst_cmds->push_back(
                    new InstCmd(InstCmd::AND, res_reg, res_reg, 0xff)
                );
            }
            break;

        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case LESS:
        case LESSOREQUAL:
        case MORE:
        case MOREOREQUAL:
        case NOTEQUAL:
        case EQUAL:
            switch(tuple->op){
                case ADD: inst_op = InstCmd::ADD; break;
                case SUB: inst_op = InstCmd::SUB; break;
                case MUL: inst_op = InstCmd::MUL; break;
                case DIV: inst_op = InstCmd::DIV; break;
                case LESS: inst_op = InstCmd::SLT; break;
                case LESSOREQUAL: inst_op = InstCmd::SLE; break;
                case MORE: inst_op = InstCmd::SGT; break;
                case MOREOREQUAL:  inst_op = InstCmd::SGE; break;
                case NOTEQUAL:  inst_op = InstCmd::SNE; break;
                case EQUAL: inst_op = InstCmd::SEQ; break;
                default: throw string("FuncBackend: something wrong.");
            }
            // load left
            if(tuple->left->type == Operand::ENTRY){
                left_reg = registAndLoadVar(tuple->left->entry, inst_cmds);
            }
            else if(tuple->left->type == Operand::INT_CONST){
                left_reg = askForTempReg(inst_cmds);
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, left_reg, back::zero, tuple->left->int_const)
                );
            }
            else if(tuple->left->type == Operand::CHAR_CONST){
                left_reg = askForTempReg(inst_cmds);
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, left_reg, back::zero, tuple->left->char_const)
                );
            }
            // regist res
            res_reg = registVar(tuple->res->entry, inst_cmds);
            // load right & calculate
            if(tuple->right->type == Operand::ENTRY){
                right_reg = registAndLoadVar(tuple->right->entry, inst_cmds);
                inst_cmds->push_back(
                    new InstCmd(inst_op, res_reg, left_reg, right_reg)
                );
            }
            else if(tuple->right->type == Operand::INT_CONST){
                inst_cmds->push_back(
                    new InstCmd(inst_op, res_reg, left_reg, tuple->right->int_const)
                );
            }
            else if(tuple->right->type == Operand::CHAR_CONST){
                inst_cmds->push_back(
                    new InstCmd(inst_op, res_reg, left_reg, tuple->right->char_const)
                );
            }
            else
                throw string("FuncBackend: Invalid tuple->right's type: " + to_string(tuple->right->type));
            // type conv
            if(tuple->res->entry->type == sym::CHAR){
                inst_cmds->push_back(
                    new InstCmd(InstCmd::AND, res_reg, res_reg, 0xff)
                );
            }
            break;


        case WARRAY:
            // calculate selector
            sel_reg = askForTempReg(inst_cmds);
            if(tuple->left->type == Operand::ENTRY){
                left_reg = registAndLoadVar(tuple->left->entry, inst_cmds);
                // sll
                inst_cmds->push_back(
                    new InstCmd(InstCmd::SLL, sel_reg, left_reg, 2)
                );
            }
            else if(tuple->left->type == Operand::INT_CONST){
                // add
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, sel_reg, back::zero, tuple->left->int_const)
                );
                // sll
                inst_cmds->push_back(
                    new InstCmd(InstCmd::SLL, sel_reg, sel_reg, 2)
                );
            }
            else
                throw string("FuncBackend: Invalid tuple->left's type: " + to_string(tuple->left->type));
            // load right
            if(tuple->right->type == Operand::ENTRY){
                right_reg = registAndLoadVar(tuple->right->entry, inst_cmds);
            }
            else if(tuple->right->type == Operand::INT_CONST){
                right_reg = askForTempReg(inst_cmds);
                // add
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, right_reg, back::zero, tuple->right->int_const)
                );
            }
            else if(tuple->right->type == Operand::CHAR_CONST){
                right_reg = askForTempReg(inst_cmds);
                // add
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, right_reg, back::zero, tuple->right->char_const)
                );
            }
            else
                throw string("FuncBackend: Invalid tuple->left's type: " + to_string(tuple->left->type));
            // save to array
            var_entry = dynamic_cast<const VarEntry*>(tuple->res->entry);
            inst_op = var_entry->type==sym::INT ? InstCmd::SW : InstCmd::SB;
            if(isGlobalVar(var_entry)){
                // sw/sb
                inst_cmds->push_back(
                    new InstCmd(inst_op, right_reg, sel_reg, NameUtil::genGlobalVarLabel(tuple->res->entry->name))
                );
            }
            else{
                // add
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, sel_reg, sel_reg, lvo_tab[NameUtil::genEntryName(tuple->res->entry)] + 4*param_count)
                );
                // add
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, sel_reg, sel_reg, back::sp)
                );
                // sw/sb
                inst_cmds->push_back(
                    new InstCmd(inst_op, right_reg, sel_reg, 0)
                );
            }
            break;
        // WARRAY

        case RARRAY:
            // calculate selector
            sel_reg = askForTempReg(inst_cmds);
            if(tuple->right->type == Operand::ENTRY){
                right_reg = registAndLoadVar(tuple->right->entry, inst_cmds);
                // sll
                inst_cmds->push_back(
                    new InstCmd(InstCmd::SLL, sel_reg, right_reg, 2)
                );
            }
            else if(tuple->right->type == Operand::INT_CONST){
                // add
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, sel_reg, back::zero, tuple->right->int_const)
                );
                // sll
                inst_cmds->push_back(
                    new InstCmd(InstCmd::SLL, sel_reg, sel_reg, 2)
                );
            }
            else
                throw string("FuncBackend: Invalid tuple->right's type: " + to_string(tuple->right->type));
            // regist res
            if(tuple->res->type == Operand::ENTRY){
                res_reg = registVar(tuple->res->entry, inst_cmds);
            }
            else
                throw string("FuncBackend: Invalid tuple->res's type: " + to_string(tuple->res->type));
            // load array
            var_entry = dynamic_cast<const VarEntry*>(tuple->left->entry);
            inst_op = var_entry->type==sym::INT ? InstCmd::LW : InstCmd::LB;
            if(isGlobalVar(var_entry)){
                // lw/lb
                inst_cmds->push_back(
                    new InstCmd(inst_op, res_reg, sel_reg, NameUtil::genGlobalVarLabel(tuple->left->entry->name))
                );
            }
            else{
                // add
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, sel_reg, sel_reg, lvo_tab[NameUtil::genEntryName(tuple->left->entry)] + 4*param_count)
                );
                // add
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, sel_reg, sel_reg, back::sp)
                );
                // lw/lb
                inst_cmds->push_back(
                    new InstCmd(inst_op, res_reg, sel_reg, 0)
                );
            }
            // type conv
            if(tuple->res->entry->type == sym::CHAR){
                inst_cmds->push_back(
                    new InstCmd(InstCmd::AND, res_reg, res_reg, 0xff)
                );
            }
            break;
        // RARRAY

        case FUNC:
            // func label
            inst_cmds->push_back(new InstCmd(NameUtil::genFuncLabel(tuple->res->str_value)));
            // adjust $sp
            inst_cmds->push_back(
                new InstCmd(InstCmd::ADD, back::sp, back::sp, -stack_size)
            );
            // save $ra
            inst_cmds->push_back(
                new InstCmd(InstCmd::SW, back::ra, back::sp, ra_offset)
            );
            // save $s?
            for(int i=0; i<gr_count; i++){
                inst_cmds->push_back(
                    new InstCmd(InstCmd::SW, 
                                back::s0+i, 
                                back::sp, 
                                ra_offset + (1+i) * SizeUtil::regSize()
                                )
                );
            }
            break;

        case PARAM:
            if(param_count == 0){
                // A function call is started
                // save this function's param regs into memory
                auto param_list = reg_pool.getAllRegistedParamReg();
                for(auto pair: param_list){
                    if(!pair.second)
                        throw string("PARAM: meet a NULL entry when saving this function's param regs.");
                    writeBackVar(pair.second, pair.first, inst_cmds);
                }
                reg_pool.resetParamReg();
            }
            // adujst $sp
            inst_cmds->push_back(
                new InstCmd(InstCmd::ADD, back::sp, back::sp, -4)
            );
            // add param count
            param_count++;
            // pass params
            if(tuple->right->int_const < back::PARAM_REG_UP - back::a0){
                // save param into param regs
                if(!reg_pool.isParamRegFree(tuple->right->int_const)){
                    // A nested function call is started
                    // save all registed param regs into memory
                    for(auto pair: reg_pool.getAllRegistedParamReg()){
                        offset = param_stacks[pair.first].top();

                        inst_cmds->push_back(
                            new InstCmd(InstCmd::SW, pair.first, back::sp, 4*param_count - offset)
                        );
                    }
                    reg_pool.resetParamReg();
                }
                // regist for the corresponding param reg
                res_reg = reg_pool.registForParamReg(tuple->right->int_const);
                // move value into the reg
                if(tuple->res->type == Operand::ENTRY){
                    left_reg = registAndLoadVar(tuple->res->entry, inst_cmds);
                    // move
                    inst_cmds->push_back(
                        new InstCmd(InstCmd::MOVE, res_reg, left_reg, 0)
                    );
                    // type conv
                    if(tuple->res->entry->type == sym::CHAR){
                        inst_cmds->push_back(
                            new InstCmd(InstCmd::AND, res_reg, res_reg, 0xff)
                        );
                    }
                }
                else if(tuple->res->type == Operand::INT_CONST){
                    // add
                    inst_cmds->push_back(
                        new InstCmd(InstCmd::ADD, res_reg, back::zero, tuple->res->int_const)
                    );
                }
                else if(tuple->res->type == Operand::CHAR_CONST){
                    // add
                    inst_cmds->push_back(
                        new InstCmd(InstCmd::ADD, res_reg, back::zero, tuple->res->char_const)
                    );
                }
                else
                    throw string("FuncBackend: Invalid tuple->res's type: " + to_string(tuple->res->type));
                // push to param's stack
                param_stacks[res_reg].push(4*param_count);
            }
            else{
                // push param into stack's memory
                if(tuple->left->str_value == NameUtil::intString)
                    inst_op = InstCmd::SW;
                else if(tuple->left->str_value == NameUtil::charString)
                    inst_op = InstCmd::SB;
                else
                    throw string("Invalid PARAM->left: " + tuple->left->str_value);
                if(tuple->res->type == Operand::ENTRY){
                    res_reg = registAndLoadVar(tuple->res->entry, inst_cmds);
                    // sw/sb
                    inst_cmds->push_back(
                        new InstCmd(inst_op, res_reg, back::sp, 0)
                    );
                }
                else if(tuple->res->type == Operand::INT_CONST){
                    res_reg = askForTempReg(inst_cmds);
                    // add
                    inst_cmds->push_back(
                        new InstCmd(InstCmd::ADD, res_reg, back::zero, tuple->res->int_const)
                    );
                    // sw/sb
                    inst_cmds->push_back(
                        new InstCmd(inst_op, res_reg, back::sp, 0)
                    );
                }
                else if(tuple->res->type == Operand::CHAR_CONST){
                    res_reg = askForTempReg(inst_cmds);
                    // add
                    inst_cmds->push_back(
                        new InstCmd(InstCmd::ADD, res_reg, back::zero, tuple->res->char_const)
                    );
                    // sw/sb
                    inst_cmds->push_back(
                        new InstCmd(inst_op, res_reg, back::sp, 0)
                    );
                }
                else
                    throw string("FuncBackend: Invalid tuple->res's type: " + to_string(tuple->res->type));
            }
            break;

        case CALL:
            if(tuple->left->int_const > 0){
                // if the function has params
                // check param regs
                param_number = tuple->left->int_const;
                if(param_number > back::PARAM_REG_UP - back::a0)
                    param_number = back::PARAM_REG_UP - back::a0;
                for(index=0; index<param_number; index++){
                    if(!reg_pool.isParamRegFree(index))
                        continue;
                    // is not assigned, need load from memory into param regs
                    offset = param_stacks[static_cast<back::REG>(index+back::a0)].top();
                    inst_cmds->push_back(
                        new InstCmd(InstCmd::LW, static_cast<back::REG>(index+back::a0), back::sp, 4*param_count - offset)
                    );
                }
                // pop from param stacks
                for(index=0; index<param_number; index++){
                    param_stacks[static_cast<back::REG>(index+back::a0)].pop();
                }
            }
            // save temp regs that has been registed by a var
            saveTempReg(inst_cmds);
            // jump and link to target function
            inst_cmds->push_back(
                new InstCmd(InstCmd::JAL, NameUtil::genFuncLabel(tuple->res->str_value))
            );
            // restore temp regs
            restoreTempReg(inst_cmds);
            // recover $sp
            if(tuple->left->int_const > 0){
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, back::sp, back::sp, 4*tuple->left->int_const)
                );
            }
            // reduce param count
            param_count -= tuple->left->int_const;
            // if called function used any register, unregist all param regs
            if(tuple->left->int_const > 0)
                reg_pool.resetParamReg();
            if(param_count == 0){
                // function call is completed, load back this function's params
                auto param_list = func_tuple->func_entry->param_list;
                index = 0;
                for(VarEntry *entry: param_list){
                    if(index >= back::PARAM_REG_UP - back::a0)
                        break;
                    if(!entry)
                        throw string("PARAM: meet a NULL entry when loading this function's param regs.");
                    // check whether has already got a temp reg
                    left_reg = reg_pool.lookUpReg(entry);
                    // regist for a param reg
                    res_reg = reg_pool.registForParamReg(index, entry);
                    if(left_reg != back::NO_REG){
                        // has already got a temp reg
                        inst_cmds->push_back(
                            new InstCmd(InstCmd::MOVE, res_reg, left_reg)
                        );
                        reg_pool.unregistTempReg(entry);
                    }
                    else{
                        loadVar(entry, res_reg, inst_cmds);
                    }
                    index++;
                }
            }
            break;

        case RET:
            // save all global vars that has registed a register
            reg_entry_map = reg_pool.getAllRegistedVar();
            for(auto pair: reg_entry_map){
                back::REG reg = pair.first;
                const VarEntry *entry = pair.second;
                if(!isGlobalVar(entry))
                    continue;

                writeBackVar(entry, reg, inst_cmds);
            }
            // load $s?
            for(int i=0; i<gr_count; i++){
                inst_cmds->push_back(
                    new InstCmd(InstCmd::LW, 
                                back::s0+i, 
                                back::sp, 
                                ra_offset + (1+i) * SizeUtil::regSize()
                                )
                );
            }
            // load $ra
            inst_cmds->push_back(
                new InstCmd(InstCmd::LW, back::ra, back::sp, ra_offset)
            );
            // recover $sp
            inst_cmds->push_back(
                new InstCmd(InstCmd::ADD, back::sp, back::sp, stack_size)
            );
            // jump return
            inst_cmds->push_back(
                new InstCmd(InstCmd::JR, back::ra)
            );
            break;
        // RET

        case INPUT:
            // get var entry
            if(tuple->res->type != Operand::ENTRY)
                throw string("FuncBackend: When process input tuple, res is a non-entry ord.");
            var_entry = dynamic_cast<const VarEntry*>(tuple->res->entry);
            if(!var_entry)
                throw string("FuncBackend: When process input tuple, res is a non-var entry.");
            // check type
            if(var_entry->type == sym::INT)
                sys_num = 5;
            else if(var_entry->type == sym::CHAR)
                sys_num = 12;
            else
                throw string("FuncBackend: When process input tuple, res's value type is invalid: " + to_string(var_entry->type));
            // save & set $v0
            writeToTempMem(back::v0, inst_cmds);
            inst_cmds->push_back(
                new InstCmd(InstCmd::ADD, back::v0, back::zero, sys_num)
            );
            // syscall
            inst_cmds->push_back(
                new InstCmd(InstCmd::SYSCALL)
            );
            // recover $v0
            loadFromTempMem(back::v0, inst_cmds);
            // regist res
            res_reg = registVar(tuple->res->entry, inst_cmds);
            // save res
            inst_cmds->push_back(
                new InstCmd(InstCmd::MOVE, res_reg, back::v0)
            );
            // type conv
            if(tuple->res->entry->type == sym::CHAR){
                inst_cmds->push_back(
                    new InstCmd(InstCmd::AND, res_reg, res_reg, 0xff)
                );
            }
            break;
        // INPUT

        case OUTPUT:
            if(tuple->left){
                // output string
                // search/create string label
                if(str_tab.find(tuple->left->str_value) != str_tab.end())
                    str_label = str_tab[tuple->left->str_value];
                else{
                    str_label = NameUtil::genUniStringLabel();
                    str_tab[tuple->left->str_value] = str_label;
                    data_cmds->push_back(
                        new DataCmd(DataCmd::ASCIIZ, str_label, tuple->left->str_value)
                    );
                }
                // set $a0
                if(func_tuple->func_entry->param_list.size() > 0){
                    // if there is a param, which means $a0 is being used
                    // save $a0
                    writeBackVar(reg_pool.lookUpEntry(back::a0), back::a0, inst_cmds);
                }
                inst_cmds->push_back(
                    new InstCmd(InstCmd::LA, back::a0, str_label)
                );
                // save & set $v0
                writeToTempMem(back::v0, inst_cmds);
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, back::v0, back::zero, 4)
                );
                // syscall
                inst_cmds->push_back(
                    new InstCmd(InstCmd::SYSCALL)
                );
                // recover $v0
                loadFromTempMem(back::v0, inst_cmds);
                // if there is a param, recover $a0
                if(func_tuple->func_entry->param_list.size() > 0){
                    loadVar(reg_pool.lookUpEntry(back::a0), back::a0, inst_cmds);
                }
            }
            if(tuple->right){
                // output expression
                // set $a0
                if(tuple->right->type == Operand::ENTRY){
                    var_entry = dynamic_cast<const VarEntry*>(tuple->right->entry);
                    // check type
                    sys_num = var_entry->type==sym::INT ? 1 : 11;
                    right_reg = registAndLoadVar(var_entry, inst_cmds);
                    // move
                    inst_cmds->push_back(
                        new InstCmd(InstCmd::MOVE, back::a0, right_reg)
                    );
                }
                else if(tuple->right->type == Operand::INT_CONST){
                    sys_num = 1;
                    // add
                    inst_cmds->push_back(
                        new InstCmd(InstCmd::ADD, back::a0, back::zero, tuple->right->int_const)
                    );
                }
                else if(tuple->right->type == Operand::CHAR_CONST){
                    sys_num = 11;
                    // add
                    inst_cmds->push_back(
                        new InstCmd(InstCmd::ADD, back::a0, back::zero, tuple->right->char_const)
                    );
                }
                // save & set $v0
                writeToTempMem(back::v0, inst_cmds);
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, back::v0, back::zero, sys_num)
                );
                // syscall
                inst_cmds->push_back(
                    new InstCmd(InstCmd::SYSCALL)
                );
                // recover $v0
                loadFromTempMem(back::v0, inst_cmds);
            }
            break;
        // OUTPUT

        case BEZ:
            // load left
            if(tuple->left->type == Operand::ENTRY){
                var_entry = dynamic_cast<const VarEntry*>(tuple->left->entry);
                left_reg = registAndLoadVar(var_entry, inst_cmds);
            }
            else if(tuple->left->type == Operand::INT_CONST){
                left_reg = askForTempReg(inst_cmds);
                // add
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, left_reg, back::zero, tuple->left->int_const)
                );
            }
            else if(tuple->left->type == Operand::CHAR_CONST){
                left_reg = askForTempReg(inst_cmds);
                // add
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, left_reg, back::zero, tuple->left->char_const)
                );
            }
            // beqz
            inst_cmds->push_back(
                new InstCmd(InstCmd::BEQZ, left_reg, tuple->res->str_value)
            );
            break;
        // BEZ

        case BEQ:
            // load left
            if(tuple->left->type == Operand::ENTRY){
                var_entry = dynamic_cast<const VarEntry*>(tuple->left->entry);
                left_reg = registAndLoadVar(var_entry, inst_cmds);
            }
            else if(tuple->left->type == Operand::INT_CONST){
                left_reg = askForTempReg(inst_cmds);
                // add
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, left_reg, back::zero, tuple->left->int_const)
                );
            }
            else if(tuple->left->type == Operand::CHAR_CONST){
                left_reg = askForTempReg(inst_cmds);
                // add
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, left_reg, back::zero, tuple->left->char_const)
                );
            }
            // load right
            if(tuple->right->type == Operand::ENTRY){
                var_entry = dynamic_cast<const VarEntry*>(tuple->right->entry);
                right_reg = registAndLoadVar(var_entry, inst_cmds);
            }
            else if(tuple->right->type == Operand::INT_CONST){
                right_reg = askForTempReg(inst_cmds);
                // add
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, right_reg, back::zero, tuple->right->int_const)
                );
            }
            else if(tuple->right->type == Operand::CHAR_CONST){
                right_reg = askForTempReg(inst_cmds);
                // add
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, right_reg, back::zero, tuple->right->char_const)
                );
            }
            // beq
            inst_cmds->push_back(
                new InstCmd(InstCmd::BEQ, left_reg, right_reg, tuple->res->str_value)
            );
            break;
        // BEQ

        case JMP:
            inst_cmds->push_back(
                new InstCmd(InstCmd::J, tuple->res->str_value)
            );
            break;
        // JMP

        default:
            throw string("FuncBackend: Not implemented tuple OP: " + to_string(tuple->op));
    }
    clearTempRegReserve();
}
