#ifndef REG_H
#define REG_H

#include <string>

namespace back
{
    enum REG{
        zero = 0,
        v0,
        a0,
        a1,
        a2,
        a3,
        PARAM_REG_UP,
        t0,
        t1,
        t2,
        t3,
        t4,
        t5,
        t6,
        t7,
        t8,
        t9,
        TEMP_REG_UP,
        s0,
        s1,
        s2,
        s3,
        s4,
        s5,
        s6,
        s7,
        GLOBAL_REG_UP,
        ra,
        sp,
        NO_REG,
    };

    REG operator++(REG &reg, int n);

    REG operator+(REG reg, int n);

    REG operator+(int n, REG reg);

    REG operator+(REG reg, unsigned int n);

    REG operator+(unsigned int n, REG reg);

    const int MAX_REG_NUM = 40;
    extern std::string REG_NAME[MAX_REG_NUM];
} // back

#endif//REG_H