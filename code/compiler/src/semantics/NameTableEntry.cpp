#include "NameTableEntry.h"
#include "NameTable.h"

namespace sem{
    char ENTRY_TYPE_NAME[MAX_ENTRY_TYPE_NUM][MAX_ENTRY_TYPE_LEN] = {
        "EMPTY_ENTRY",
        "VAR_ENTRY",
        "FUN_ENTRY",
    };
}

string NameTableEntry::getOwnerName()const{
    if(owner)
        return owner->func_name;
    else
        return "";
}

string VarEntry::toString(){
    string s = NameTableEntry::toString();
    s += " ";
    s += sym::SYMBOL_NAME[type];
    s += " [" + to_string(dim) + "]";
    if(is_const){
        s += " CONST";
        switch(type){
            case sym::INT:
                s += " " + to_string(int_value);
                break;
            case sym::CHAR:
                s += " " + string(1, char_value);
                break;
            default:
                mylog::error << "const entry: invalid type [" << type << "]";
                exit(-1);
        }
    }
    if(is_param)
        s+= " PARAM(" + to_string(param_index) + ")";
    return s;
}
