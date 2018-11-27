#ifndef REG_H
#define REG_H

#include <string>

namespace back
{
    enum REG{
        zero = 0,
        v0,
        a0,
        t0,
        t1,
        t2,
        t3,
        t4,
        t5,
        t6,
        t7,
        TEMP_REG_UP,
        ra,
        sp,
        NO_REG,
    };

    const int MAX_REG_NUM = 40;
    extern std::string REG_NAME[MAX_REG_NUM];
} // back

#endif//REG_H