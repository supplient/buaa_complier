#include "NameUtil.h"
#include "NameTableEntry.h"
#include "config.h"

string NameUtil::intString = "INT";
string NameUtil::charString = "CHAR";

string NameUtil::genEntryName(const NameTableEntry *entry){
    if(FUNC_PREFIX_ENTRY_NAME)
        return "%" + entry->getOwnerName() + "%" + entry->name;
    else
        return entry->name;
}

string NameUtil::genEntryName(const VarEntry *entry){
    return genEntryName(static_cast<const NameTableEntry*>(entry));
}
