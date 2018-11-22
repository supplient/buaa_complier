#ifndef OPERAND_H
#define OPERAND_H

#include <string>

class NameTableEntry;

using namespace std;

class Operand
{
public:
    enum TYPE{
        ENTRY,
        INT_CONST,
        CHAR_CONST,
        // TODO
    };

    Operand(NameTableEntry *entry): type(ENTRY), entry(entry), int_const(0), char_const(0)
    {}
    Operand(int int_value): type(INT_CONST), entry(NULL), int_const(int_value), char_const(0)
    {}
    Operand(char char_value): type(CHAR_CONST), entry(NULL), int_const(0), char_const(char_value)
    {}

    string toString(){
        // TODO
        return "";
    }

    const TYPE type;
    const NameTableEntry *entry;
    const int int_const;
    const char char_const;
};

#endif//OPERAND_H
