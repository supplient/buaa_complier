#include "Tuple.h"

namespace sem{
    char TUPLE_OP_NAME[MAX_OP_NUM][MAX_OP_LEN] = {
        "EMPTY",
        "LABEL",
        "ASSIGN",
        "NEG",
        "ADD",
        "SUB",
        "MUL",
        "DIV",
        "WARRAY",
        "RARRAY",
        "RET",
        "PARAM",
        "CALL",
        "INPUT",
        "OUTPUT",
        "LESS",
        "LESSOREQUAL",
        "MORE",
        "MOREOREQUAL",
        "NOTEQUAL",
        "EQUAL",
        "BEZ",
        "JMP",
    };
}