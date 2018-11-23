#include "NameUtil.h"
#include "NameTableEntry.h"
#include "config.h"

string NameUtil::genEntryName(const NameTableEntry *entry){
    if(FUNC_PREFIX_ENTRY_NAME)
        return "%" + entry->getOwnerName() + "%" + entry->name;
    else
        return entry->name;
}