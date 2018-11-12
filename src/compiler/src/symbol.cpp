#include "symbol.h"
namespace sym{
    char SYMBOL_NAME[MAX_SYMBOL_NUM][MAX_SYMBOL_LEN] = {
        //EOF
        "ENDOFFILE",

        //OPERATOR
        "PLUS",
        "MINUS",
        "MULTI",
        "DIV",
        "LESS",
        "LESSOREQUAL",
        "MORE",
        "MOREOREQUAL",
        "NOTEQUAL",
        "EQUAL",
        "ASSIGN",

        //SEPARATOR
        "LEFT_ROUND",
        "RIGHT_ROUND",
        "LEFT_SQUARE",
        "RIGHT_SQUARE",
        "LEFT_BRACE",
        "RIGHT_BRACE",
        "SEMICOLON",
        "COLON",
        "COMMA",

        //UNSIGNED_INTEGER
        "UNSIGNED_INTEGER",

        //CHARACTER
        "CHARACTER",

        //IDENTIFIER
        "IDENTIFIER",

        //RESERVED
        "CONST",
        "INT",
        "CHAR",
        "VOID",
        "MAIN",
        "IF",
        "WHILE",
        "SWITCH",
        "CASE",
        "DEFAULT",
        "SCANF",
        "PRINTF",
        "RETURN",

        //STRING
        "STRING",
    };
};

