#ifndef COMMAND_H
#define COMMAND_H

#include <string>

using namespace std;

class DataCmd
{
public:
    enum OP{
        ALIGN, // to align to word
        ASCIIZ, // for string
        BYTE, // for char
        WORD, // for int
        SPACE, // for array
    };

    string label;
    OP op;
    unsigned int size;
    string str_value;
};

class InstCmd
{
public:
    enum OP{

    };

};

#endif//COMMAND_H