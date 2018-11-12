#include <iostream>
#include <string>

#include "symbol.h"
#include "LexAnalyzer.h"

using namespace std;

void lexOutputSym(const LexAnalyzer &lex, sym::SYMBOL sy){
    cout << lex.getLineNo() << "[" << lex.getColumnNo() << "]: "
         << sym::SYMBOL_NAME[sy];
    if(sy == sym::IDENTIFIER){
        cout << "\t" << lex.getStringValue();
    }
    cout << endl;
}

int main()
{
    string filename = "test.txt";

    LexAnalyzer lex(filename);
    sym::SYMBOL sy;

    sy = lex.nextSymbol();
    lexOutputSym(lex, sy);
    sy = lex.nextSymbol();
    lexOutputSym(lex, sy);
    if(lex.goBack())
        cout << "Go backed." << endl;
    else
        cout << "Go back failed." << endl;
    if(lex.goBack())
        cout << "Go backed." << endl;
    else
        cout << "Go back failed." << endl;
    if(lex.goBack())
        cout << "Go backed." << endl;
    else
        cout << "Go back failed." << endl;

    while( (sy=lex.nextSymbol()) != sym::ENDOFFILE ){
        lexOutputSym(lex, sy);
    }

    cout << "Hello world!" << endl;
    return 0;
}
