#ifndef COMMAND_H
#define COMMAND_H

#include <string>

#include "Reg.h"

using namespace std;

class DataCmd
{
public:
    enum OP{
        ASCIIZ, // for string
        WORD, // for int
        SPACE, // for array
    };

    string toString();

    string label;
    OP op;
    unsigned int size;
    string str_value;
};

class InstCmd
{
public:
    enum OP{
        NONE,
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
        BEQ,
    };
    static const int MAX_OP_NUM = 100;
    static string OP_NAME[MAX_OP_NUM];

    enum RIGHT_TYPE{
        REG_TYPE,
        IMME_TYPE,
        LABEL_TYPE,
    };

    InstCmd(){has_label = false;}

    InstCmd(string label){
        has_label = true;
        this->label = label;
        this->op = NONE;
        res_reg = back::NO_REG;
        left_reg = back::NO_REG;
    }

    InstCmd(OP op){
        has_label = false;
        this->op = op;
        res_reg = back::NO_REG;
        left_reg = back::NO_REG;
    }

    InstCmd(OP op, back::REG res){
        has_label = false;
        this->op = op;
        res_reg = res;
        left_reg = back::NO_REG;
    }

    InstCmd(OP op, string right){
        has_label = false;
        this->op = op;
        res_reg = back::NO_REG;
        left_reg = back::NO_REG;
        right_type = LABEL_TYPE;
        right_label = right;
    }

    InstCmd(OP op, back::REG res, back::REG left){
        has_label = false;
        this->op = op;
        res_reg = res;
        left_reg = left;
    }

    InstCmd(OP op, back::REG res, back::REG left, back::REG right){
        has_label = false;
        this->op = op;
        res_reg = res;
        left_reg = left;
        right_type = REG_TYPE;
        right_reg = right;
    }

    InstCmd(OP op, back::REG res, back::REG left, int right){
        has_label = false;
        this->op = op;
        res_reg = res;
        left_reg = left;
        right_type = IMME_TYPE;
        right_imme = right;
    }

    // reg to string
    string rts(back::REG reg){
        return back::REG_NAME[reg];
    }
    string rightToString(){
        switch(right_type){
            case REG_TYPE:
                return rts(right_reg);
            case IMME_TYPE:
                return to_string(right_imme);
            case LABEL_TYPE:
                return right_label;
        }
        return string();
    }
    string toString();

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
