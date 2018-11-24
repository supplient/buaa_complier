#ifndef OPERAND_H
#define OPERAND_H

#include <string>

#include "NameUtil.h"

class NameTableEntry;
class Tuple;

using namespace std;

class Operand
{
public:
    enum TYPE{
        ENTRY,
        INT_CONST,
        CHAR_CONST,
        STRING,
        // TODO
    };

    Operand(NameTableEntry *entry): type(ENTRY), entry(entry), int_const(0), char_const(0), str_value("")
    {}
    Operand(int int_value): type(INT_CONST), entry(NULL), int_const(int_value), char_const(0), str_value("")
    {}
    Operand(char char_value): type(CHAR_CONST), entry(NULL), int_const(0), char_const(char_value), str_value("")
    {}
    Operand(string str_value): type(STRING), entry(NULL), int_const(0), char_const(0), str_value(str_value)
    {}

    string toString();

    const TYPE type;

    const NameTableEntry *entry;
    const int int_const;
    const char char_const;
    const string str_value;
};

#endif//OPERAND_H
