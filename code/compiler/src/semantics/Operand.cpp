#include "Operand.h"

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