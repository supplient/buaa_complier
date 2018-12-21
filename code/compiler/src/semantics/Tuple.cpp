#include "Tuple.h"
#include "NameTable.h"

namespace sem{
    char TUPLE_OP_NAME[MAX_OP_NUM][MAX_OP_LEN] = {
        "EMPTY",
        "LABEL",
        "ASSIGN",
        "NEG",
        "ADD",
        "SUB",
        "MUL",
        "DIV",
        "WARRAY",
        "RARRAY",
        "FUNC",
        "PARAM",
        "CALL",
        "RET",
        "INPUT",
        "OUTPUT",
        "LESS",
        "LESSOREQUAL",
        "MORE",
        "MOREOREQUAL",
        "NOTEQUAL",
        "EQUAL",
        "BEZ",
        "BEQ",
        "JMP",
    };
}

string Tuple::toString(NameTable &tab)const{
    string s = "";

    if(op==sem::FUNC){
        const FuncEntry *func_entry = tab.lookUpFunc(res->str_value);
        switch(func_entry->return_type){
            case sym::INT: s += "int"; break;
            case sym::CHAR: s += "char"; break;
            case sym::VOID: s += "void"; break;
            default: throw string("invalid return type: " + to_string(func_entry->return_type));
        }
        s += " " +  func_entry->name + "()";
        for(const VarEntry *param: func_entry->param_list){
            s += "\npara ";
            switch(param->type){
                case sym::INT: s += "int"; break;
                case sym::CHAR: s += "char"; break;
                case sym::VOID: s += "void"; break;
                default: throw string("invalid param type: " + to_string(param->type));
            }
            s += " " + param->name;
        }

        FuncNameTable *func_tab = tab.getFuncNameTable(res->str_value);
        for(const VarEntry *var: func_tab->getUnconstVars()){
            if(var->is_param)
                continue;
            s += "\nvar ";
            switch(var->type){
                case sym::INT: s += "int"; break;
                case sym::CHAR: s += "char"; break;
                case sym::VOID: s += "void"; break;
                default: throw string("invalid var type: " + to_string(var->type));
            }
            s += " " + var->name;
        }
    }
    else if(op == sem::LABEL)
        s += res->str_value + ":";
    else if(op == sem::ASSIGN)
        s += res->toString() + " = " + left->toString();
    else if(op == sem::NEG)
        s += res->toString() + " = -" + left->toString();
    else if(op == sem::ADD)
        s += res->toString() + " = " + left->toString() + " + " + right->toString();
    else if(op == sem::SUB)
        s += res->toString() + " = " + left->toString() + " - " + right->toString();
    else if(op == sem::MUL)
        s += res->toString() + " = " + left->toString() + " * " + right->toString();
    else if(op == sem::DIV)
        s += res->toString() + " = " + left->toString() + " / " + right->toString();
    else if(op == sem::WARRAY)
        s += res->toString() + "[" + left->toString() + "] = " + right->toString();
    else if(op == sem::RARRAY)
        s += res->toString() + " = " + left->toString() + "[" + right->toString() + "]";
    else if(op == sem::PARAM)
        s += "push " + res->toString();
    else if(op == sem::CALL)
        s += "call " + res->str_value;
    else if(op == sem::RET)
        s += "ret";
    else if(op == sem::INPUT)
        s += "input " + res->toString();
    else if(op == sem::OUTPUT){
        if(left)
            s += "output " + left->toString();
        if(right){
            if(left) s += "\n";
            s += "output " + right->toString();
        }
    }
    else if(op == sem::LESS)
        s += res->toString() + " = " + left->toString() + " < " + right->toString();
    else if(op == sem::LESSOREQUAL)
        s += res->toString() + " = " + left->toString() + " <= " + right->toString();
    else if(op == sem::MORE)
        s += res->toString() + " = " + left->toString() + " > " + right->toString();
    else if(op == sem::MOREOREQUAL)
        s += res->toString() + " = " + left->toString() + " >= " + right->toString();
    else if(op == sem::NOTEQUAL)
        s += res->toString() + " = " + left->toString() + " != " + right->toString();
    else if(op == sem::EQUAL)
        s += res->toString() + " = " + left->toString() + " == " + right->toString();
    else if(op == sem::BEZ)
        s += "bez " + left->toString() + " " + res->toString();
    else if(op == sem::BEQ)
        s += "beq " + left->toString() + " " + right->toString() + " " + res->toString();
    else if(op == sem::JMP)
        s += "jmp " + res->toString();
    else
        throw string("invalid tuple op: " + to_string(op));
    
    return s;
}

string Tuple::toString()const{
    string s = sem::TUPLE_OP_NAME[op];
    s += " ";
    if(res)
        s += res->toString() + " ";
    if(left)
        s += left->toString() + " ";
    if(right)
        s += right->toString() + " ";
    return s;
}