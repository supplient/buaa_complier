#include "LexAnalyzer.h"

bool LexAnalyzer::hasInit = false;
map<string, sym::SYMBOL> LexAnalyzer::reservedTable;

bool LexAnalyzer::initStaticData(){
    // Fill reservedTable
    reservedTable["const"]   = sym::CONST;
    reservedTable["int"]     = sym::INT;
    reservedTable["char"]    = sym::CHAR;
    reservedTable["void"]    = sym::VOID;
    reservedTable["main"]    = sym::MAIN;
    reservedTable["if"]      = sym::IF;
    reservedTable["while"]   = sym::WHILE;
    reservedTable["switch"]  = sym::SWITCH;
    reservedTable["case"]    = sym::CASE;
    reservedTable["default"] = sym::DEFAULT;
    reservedTable["scanf"]   = sym::SCANF;
    reservedTable["printf"]  = sym::PRINTF;
    reservedTable["return"]  = sym::RETURN;

    hasInit = true;
    return true;
}