#include "FuncBackend.h"

#include <algorithm>

#include "symbol.h"
#include "log.h"

bool isGlobalVar(const VarEntry *entry);

void FuncBackend::transTuple(Tuple *tuple, map<string, string> &str_tab,
    vector<DataCmd*> *data_cmds, vector<InstCmd*> *inst_cmds){
    using namespace sem;

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
            break;

        case ADD:
        case SUB:
        case MUL:
        case DIV:
            switch(tuple->op){
                case ADD: inst_op = InstCmd::ADD; break;
                case SUB: inst_op = InstCmd::SUB; break;
                case MUL: inst_op = InstCmd::MUL; break;
                case DIV: inst_op = InstCmd::DIV; break;
                default: throw string("FuncBackend: something wrong.");
            }
            left_reg = registAndLoadVar(tuple->left->entry, inst_cmds);
            res_reg = registVar(tuple->res->entry, inst_cmds);
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
            break;

        case LESS:
        case LESSOREQUAL:
        case MORE:
        case MOREOREQUAL:
        case NOTEQUAL:
        case EQUAL:
            switch(tuple->op){
                case LESS: inst_op = InstCmd::SLT; break;
                case LESSOREQUAL: inst_op = InstCmd::SLE; break;
                case MORE: inst_op = InstCmd::SGT; break;
                case MOREOREQUAL:  inst_op = InstCmd::SGE; break;
                case NOTEQUAL:  inst_op = InstCmd::SNE; break;
                case EQUAL: inst_op = InstCmd::SEQ; break;
                default: throw string("FuncBackend: something wrong.");
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
            else
                throw string("FuncBackend: Invalid tuple->right's type: " + to_string(tuple->right->type));
            // load left
            left_reg = registAndLoadVar(tuple->left->entry, inst_cmds);
            // regist res
            res_reg = registVar(tuple->res->entry, inst_cmds);
            // op
            inst_cmds->push_back(
                new InstCmd(inst_op, res_reg, left_reg, right_reg)
            );
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
                    new InstCmd(inst_op, right_reg, sel_reg, NameUtil::genEntryName(tuple->res->entry))
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
            // save to array
            var_entry = dynamic_cast<const VarEntry*>(tuple->left->entry);
            inst_op = var_entry->type==sym::INT ? InstCmd::LW : InstCmd::LB;
            if(isGlobalVar(var_entry)){
                // lw/lb
                inst_cmds->push_back(
                    new InstCmd(inst_op, res_reg, sel_reg, NameUtil::genEntryName(tuple->left->entry))
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
            break;
        // RARRAY

        case FUNC:
            // func label
            inst_cmds->push_back(new InstCmd(tuple->left->str_value));
            // adjust $sp
            inst_cmds->push_back(
                new InstCmd(InstCmd::ADD, back::sp, back::sp, -stack_size)
            );
            // save $ra
            inst_cmds->push_back(
                new InstCmd(InstCmd::SW, back::ra, back::sp, ra_offset)
            );
            // save $s?
            // TODO
            break;

        case PARAM:
            // adujst $sp
            inst_cmds->push_back(
                new InstCmd(InstCmd::ADD, back::sp, back::sp, -4)
            );
            // add param count
            param_count++;
            // push param
            if(tuple->res->type == Operand::ENTRY){
                res_reg = registAndLoadVar(tuple->res->entry, inst_cmds);
                // sw
                inst_cmds->push_back(
                    new InstCmd(InstCmd::SW, res_reg, back::sp, 0)
                );
            }
            else if(tuple->res->type == Operand::INT_CONST){
                res_reg = askForTempReg(inst_cmds);
                // add
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, res_reg, back::zero, tuple->res->int_const)
                );
                // sw
                inst_cmds->push_back(
                    new InstCmd(InstCmd::SW, res_reg, back::sp, 0)
                );
            }
            else if(tuple->res->type == Operand::CHAR_CONST){
                res_reg = askForTempReg(inst_cmds);
                // add
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, res_reg, back::zero, tuple->res->char_const)
                );
                // sw
                inst_cmds->push_back(
                    new InstCmd(InstCmd::SW, res_reg, back::sp, 0)
                );
            }
            else
                throw string("FuncBackend: Invalid tuple->res's type: " + to_string(tuple->res->type));
            break;

        case CALL:
            // save temp regs that has been registed by a var
            saveTempReg(inst_cmds);
            // jump and link to target function
            inst_cmds->push_back(
                new InstCmd(InstCmd::JAL, tuple->res->str_value)
            );
            // recover $sp
            inst_cmds->push_back(
                new InstCmd(InstCmd::ADD, back::sp, back::sp, 4*param_count)
            );
            // reduce param count
            param_count -= tuple->left->int_const;
            // restore temp regs
            restoreTempReg(inst_cmds);
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
            // TODO
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
            // set $v0
            inst_cmds->push_back(
                new InstCmd(InstCmd::ADD, back::v0, back::zero, sys_num)
            );
            // syscall
            inst_cmds->push_back(
                new InstCmd(InstCmd::SYSCALL)
            );
            // regist res
            res_reg = registVar(tuple->res->entry, inst_cmds);
            // save res
            inst_cmds->push_back(
                new InstCmd(InstCmd::MOVE, res_reg, back::v0)
            );
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
                inst_cmds->push_back(
                    new InstCmd(InstCmd::LA, back::a0, str_label)
                );
                // set $v0
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, back::v0, back::zero, 4)
                );
                // syscall
                inst_cmds->push_back(
                    new InstCmd(InstCmd::SYSCALL)
                );
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
                // set $v0
                inst_cmds->push_back(
                    new InstCmd(InstCmd::ADD, back::v0, back::zero, sys_num)
                );
                // syscall
                inst_cmds->push_back(
                    new InstCmd(InstCmd::SYSCALL)
                );
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
