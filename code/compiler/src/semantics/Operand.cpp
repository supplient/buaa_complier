#include "Operand.h"
#include "NameTableEntry.h"

Operand::Operand(NameTableEntry *entry): type(ENTRY), entry(dynamic_cast<const VarEntry*>(entry)), int_const(0), char_const(0), str_value("")
{
    if(this->entry == NULL)
        throw string("Operand cannot receive non-var entry as a entry value.");
}

string Operand::toString(){
    string s = "";
    switch(type){
        case ENTRY: s += NameUtil::genEntryName(entry); break;
        case INT_CONST: s += to_string(int_const); break;
        case CHAR_CONST: s += "'" + string(1, char_const) + "'"; break;
        case STRING: s += "\"" + str_value + "\""; break;
        default: throw string("Invalid Operand type: " + to_string(type));
    }
    return s;
}
