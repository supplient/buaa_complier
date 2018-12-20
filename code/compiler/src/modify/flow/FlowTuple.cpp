#include "FlowTuple.h"
#include "FlowFuncBlock.h"
#include "NameTableEntry.h"

FlowTuple::FlowTuple(Tuple *tuple)
    :tuple(tuple)
{
    this->index = FlowFuncBlock::tuple_index++;
}

vector<const VarEntry*> FlowTuple::use(){
    vector<const VarEntry*> res;
    vector<Operand*> ords;

    switch(tuple->op){
        case sem::ASSIGN:
        case sem::NEG:
        case sem::BEZ:
            ords.push_back(tuple->left);
            break;
        case sem::ADD:
        case sem::SUB:
        case sem::MUL:
        case sem::DIV:
        case sem::WARRAY:
        case sem::RARRAY:
        case sem::LESS:
        case sem::LESSOREQUAL:
        case sem::MORE:
        case sem::MOREOREQUAL:
        case sem::NOTEQUAL:
        case sem::EQUAL:
        case sem::BEQ:
            ords.push_back(tuple->left);
            ords.push_back(tuple->right);
            break;
        case sem::PARAM:
            ords.push_back(tuple->res);
            break;
        case sem::OUTPUT:
            if(tuple->left)
                ords.push_back(tuple->left);
            if(tuple->right)
                ords.push_back(tuple->right);
            break;
        case sem::LABEL:
        case sem::FUNC:
        case sem::CALL:
        case sem::RET:
        case sem::INPUT:
        case sem::JMP:
            ;// Do Nothing.
            break;
        default:
            throw string("FlowTuple.use: Not implemented " + to_string(tuple->op));
    }

    for(Operand *ord: ords){
        if(ord->type == Operand::ENTRY){
            // Note: We do not count global vars
            if(VarEntry::isGlobalVar(ord->entry))
                continue;
            res.push_back(ord->entry);
        }
    }

    return res;
}

vector<const VarEntry*> FlowTuple::def(){
    vector<const VarEntry*> res;
    Operand *ord;

    switch(tuple->op){
        case sem::ASSIGN:
        case sem::NEG:
        case sem::ADD:
        case sem::SUB:
        case sem::MUL:
        case sem::DIV:
        case sem::WARRAY:
        case sem::RARRAY:
        case sem::INPUT:
        case sem::LESS:
        case sem::LESSOREQUAL:
        case sem::MORE:
        case sem::MOREOREQUAL:
        case sem::NOTEQUAL:
        case sem::EQUAL:
            ord = tuple->res;
            break;
        case sem::LABEL:
        case sem::FUNC:
        case sem::PARAM:
        case sem::CALL:
        case sem::RET:
        case sem::OUTPUT:
        case sem::BEZ:
        case sem::BEQ:
        case sem::JMP:
            ;// Do nothing.
            break;
        default:
            throw string("FlowTuple.def: Not implemented " + to_string(tuple->op));
    }
        
    if(ord->type == Operand::ENTRY){
        // Note: We do not count global vars
        if(!VarEntry::isGlobalVar(ord->entry))
            res.push_back(ord->entry);
    }

    return res;
}