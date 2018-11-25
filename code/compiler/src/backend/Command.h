#ifndef COMMAND_H
#define COMMAND_H

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

    string name;
    OP op;
    unsigned int size;
};

class InstCmd
{
public:
    enum OP{

    };

};

#endif//COMMAND_H