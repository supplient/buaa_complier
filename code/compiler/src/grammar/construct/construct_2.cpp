#include "GrammarAnalyzer.h"

#include "symbol.h"

using namespace std;

Integer* GrammarAnalyzer::constructInteger(const SymSet &delimiter){
    // TODO test this
    const string ehd = "integer: ";
    bool minus = false;

    Integer *integer = new Integer();
    switch(*lex){
        case sym::PLUS: minus = false; lex.nextSymbol(); break;
        case sym::MINUS: minus = true; lex.nextSymbol(); break;
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

    return integer;
}