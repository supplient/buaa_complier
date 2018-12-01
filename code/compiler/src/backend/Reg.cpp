#include "Reg.h"

namespace back{
    std::string REG_NAME[MAX_REG_NUM] = {
        "$zero",
        "$v0",
        "$a0",
        "$a1",
        "$a2",
        "$a3",
        "PARAM_REG_UP",
        "$t0",
        "$t1",
        "$t2",
        "$t3",
        "$t4",
        "$t5",
        "$t6",
        "$t7",
        "$t8",
        "$t9",
        "TEMP_REG_UP",
        "$s0",
        "$s1",
        "$s2",
        "$s3",
        "$s4",
        "$s5",
        "$s6",
        "$s7",
        "GLOBAL_REG_UP",
        "$ra",
        "$sp",
        "NO_REG",
    };

    REG operator++(REG &reg, int n){
        REG ret = reg;
        reg = static_cast<REG>(reg+1);
        return ret;
    }

    REG operator+(REG reg, int n){
        return static_cast<REG>((int)reg+n);
    }

    REG operator+(int n, REG reg){
        return reg + n;
    }

    REG operator+(REG reg, unsigned int n){
        return static_cast<REG>((unsigned int)reg+n);
    }

    REG operator+(unsigned int n, REG reg){
        return reg + n;
    }
}
