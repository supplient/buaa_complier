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
        "TEMP_REG_UP",
        "$ra",
        "$sp",
        "NO_REG",
    };
}