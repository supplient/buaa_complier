#ifndef COMMAND_H
#define COMMAND_H

#include <string>

#include "Reg.h"

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
        NOP,

        LA,
        LI,
        MOVE,
        NEG,

        ADD,
        SUB,
        MUL,
        DIV,

        SLL,

        SB,
        LB,
        SW,
        LW,

        J,
        JAL,
        JR,

        SYSCALL,

        SLT,
        SLE,
        SGT,
        SGE,
        SNE,
        SEQ,

        BEQZ,
        BEQ
    };

    enum RIGHT_TYPE{
        REG_TYPE,
        IMME_TYPE,
        LABEL_TYPE,
    };

    InstCmd(){has_label = false;}

    bool has_label;
    string label;

    OP op;
    back::REG res_reg;
    back::REG left_reg;

    RIGHT_TYPE right_type;
    back::REG right_reg;
    int right_imme;
    string right_label;
};

#endif//COMMAND_H