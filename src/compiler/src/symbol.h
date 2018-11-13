#ifndef SYMBOL_H
#define SYMBOL_H

namespace sym{
    const int MAX_SYMBOL_NUM = 500;
    const int MAX_SYMBOL_LEN = 30;
    enum SYMBOL{
        //EOF
        ENDOFFILE = 0,

        //OPERATOR
        PLUS,
        MINUS,
        MULTI,
        DIV,
        LESS,
        LESSOREQUAL,
        MORE,
        MOREOREQUAL,
        NOTEQUAL,
        EQUAL,
        ASSIGN,

        //SEPARATOR
        LEFT_ROUND,
        RIGHT_ROUND,
        LEFT_SQUARE,
        RIGHT_SQUARE,
        LEFT_BRACE,
        RIGHT_BRACE,
        SEMICOLON,
        COLON,
        COMMA,

        //RESERVED
        CONST,
        INT,
        CHAR,
        VOID,
        MAIN,
        IF,
        WHILE,
        SWITCH,
        CASE,
        DEFAULT,
        SCANF,
        PRINTF,
        RETURN,

        //STRING
        STRING,

        //UNSIGNED_INTEGER
        UNSIGNED_INTEGER,

        //CHARACTER
        CHARACTER,

        //IDENTIFIER
        IDENTIFIER,
    };

    extern char SYMBOL_NAME[MAX_SYMBOL_NUM][MAX_SYMBOL_LEN];
};

#endif//SYMBOL_H
