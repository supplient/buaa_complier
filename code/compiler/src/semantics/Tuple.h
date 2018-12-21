#ifndef TUPLE_H
#define TUPLE_H

#include <iostream>
#include <vector>

#include "Operand.h"

using namespace std;

namespace sem{

    enum OP{
        EMPTY = 0,
        LABEL,
        ASSIGN,
        NEG,
        ADD,
        SUB,
        MUL,
        DIV,
        WARRAY,
        RARRAY,
        FUNC,
        PARAM,
        CALL,
        RET,
        INPUT,
        OUTPUT,
        LESS,
        LESSOREQUAL,
        MORE,
        MOREOREQUAL,
        NOTEQUAL,
        EQUAL,
        BEZ,
        BEQ,
        JMP,
    };

    const int MAX_OP_NUM = 100;
    const int MAX_OP_LEN = 30;

    extern char TUPLE_OP_NAME[MAX_OP_NUM][MAX_OP_LEN];
}

class NameTable;

class Tuple
{
public:
    Tuple(){
        op = sem::EMPTY;
        left = NULL;
        right = NULL;
        res = NULL;
    }

    string toString(NameTable &tab)const;
    string toString()const;

    sem::OP op;
    Operand *left;
    Operand *right;
    Operand *res;
};

typedef vector<Tuple*> Tuples;

#endif//TUPLE_H
