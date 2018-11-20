#include "GrammarAnalyzer.h"

#include "symbol.h"

using namespace std;

Integer* GrammarAnalyzer::constructInteger(const SymSet &delimiter){
    const string ehd = "integer: ";
    bool minus = false;
    int start_line = lex.getLineNo();

    Integer *integer = new Integer();
    switch(*lex){
        case sym::PLUS: minus = false; lex.nextSymbol(); break;
        case sym::MINUS: minus = true; lex.nextSymbol(); break;
        default: ;
    }

    if(*lex != sym::UNSIGNED_INTEGER){
        errorRepo("should take an integer.");
        skip(sym::UNSIGNED_INTEGER, delimiter);
    }

    if(*lex == sym::UNSIGNED_INTEGER){
        integer->value = lex.getIntValue();
        if(minus)
            integer->value = -integer->value;
        lex.nextSymbol();
    }
    else{
        delete integer;
        integer = NULL;
    }

    int end_line = lex.getLineNo();
    if(integer){
        integer->start_line = start_line;
        integer->end_line = end_line;
    }

    #if HW
    if(integer != NULL)
        log::hw << "integer";
    #endif//HW

    return integer;
}
