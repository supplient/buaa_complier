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
