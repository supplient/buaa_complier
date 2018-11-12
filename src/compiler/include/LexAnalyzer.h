#ifndef LEX_ANALYZER_H
#define LEX_ANALYZER_H

#include <fstream>
#include <string>
#include <stack>

#include "symbol.h"
using namespace std;

class LexAnalyzer
{
    public:
        LexAnalyzer(string filename);
        virtual ~LexAnalyzer();

        sym::SYMBOL nextSymbol();
        bool goBack();

        int getLineNo()const{return history.top().line;}
        int getColumnNo()const{return history.top().column;}

        int getIntValue()const{return int_value;}
        char getCharValue()const{return char_value;}
        string getStringValue()const{return string_value;}

    private:
        class State{
            public:
                unsigned int int_value;
                char char_value;
                string string_value;
                sym::SYMBOL symbol;
                char ch;
                int line;
                int column;
                streampos pos;
        };

        unsigned int int_value;
        char char_value;
        string string_value;
        sym::SYMBOL symbol;

        char ch;
        unsigned int line;
        unsigned int column;
        ifstream file;

        stack<State> history;
        State getCurrentState();
        void loadState(const State &);

        bool _nextSymbol(sym::SYMBOL &sy);

        bool isEOF();
        bool getChar(char &c);
        
        bool isAlpha(char c);
        bool isNotZeroDigit(char c);
        bool isDigit(char c);
};

#endif // LEX_ANALYZER_H
