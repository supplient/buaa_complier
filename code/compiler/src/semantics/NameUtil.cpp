#include "NameUtil.h"
#include "NameTableEntry.h"

string NameUtil::genEntryName(const NameTableEntry *entry){
    return "%" + entry->getOwnerName() + "%" + entry->name;
}