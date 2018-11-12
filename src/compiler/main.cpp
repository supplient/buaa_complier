#include <iostream>
#include <string>

#include "symbol.h"
#include "LexAnalyzer.h"

using namespace std;

void lexOutputSym(const LexAnalyzer &lex, sym::SYMBOL sy){
    cout << lex.getLineNo() << "[" << lex.getColumnNo() << "]: "
         << sym::SYMBOL_NAME[sy];
    switch(sy){
        case sym::IDENTIFIER:
            cout << "\t" << lex.getStringValue();
            break;
        case sym::UNSIGNED_INTEGER:
            cout << "\t" << lex.getIntValue();
            break;
        case sym::CHARACTER:
            cout << "\t" << lex.getCharValue();
            break;
        case sym::STRING:
            cout << "\t" << lex.getStringValue();
            break;
    }
    cout << endl;
}

int main()
{
    string filename = "example.c0";

    LexAnalyzer lex(filename);
    sym::SYMBOL sy;

    while( (sy=lex.nextSymbol()) != sym::ENDOFFILE ){
        lexOutputSym(lex, sy);
    }

    cout << "Hello world!" << endl;
    return 0;
}
