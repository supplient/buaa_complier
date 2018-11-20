#ifndef TUPLE_H
#define TUPLE_H

#include <iostream>
#include <vector>

#include "Operand.h"

using namespace std;

namespace sem{

    enum OP{
        EMPTY = 0,
        // TODO
    };

    const int MAX_OP_NUM = 100;
    const int MAX_OP_LEN = 30;

    extern char TUPLE_OP_NAME[MAX_OP_NUM][MAX_OP_LEN];
}

class Tuple
{
public:
    Tuple(){
        op = sem::EMPTY;
        left = NULL;
        right = NULL;
        res = NULL;
    }

    sem::OP op;
    Operand *left;
    Operand *right;
    Operand *res;

    string toString(){
        string s = sem::TUPLE_OP_NAME[op];
        s += " ";
        if(res)
            s += res->toString() + " ";
        if(left)
            s += left->toString() + " ";
        if(right)
            s += right->toString() + " ";
        return s;
    }
};

typedef vector<Tuple*> Tuples;

#endif//TUPLE_H
