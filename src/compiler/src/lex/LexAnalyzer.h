#ifndef LEX_ANALYZER_H
#define LEX_ANALYZER_H

#include <map>
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

        // Static function
        static bool initStaticData();

    private:
        // Static Data
        static bool hasInit;
        static map<string, sym::SYMBOL> reservedTable;

        // Internal Structure
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

        // Symbol state
        unsigned int int_value;
        char char_value;
        string string_value;
        sym::SYMBOL symbol;

        // Char state
        char ch;
        unsigned int line;
        unsigned int column;
        ifstream file;

        // State history
        stack<State> history;
        State getCurrentState();
        void loadState(const State &);

        // Error report
        void errorRepo(string reason);

        // Internal symbol solve
        bool _nextSymbol(sym::SYMBOL &sy);

        // Char control
        bool isEOF();
        bool getChar(char &c);

        // Lex judge support
        bool isAlpha(char c);
        bool isNotZeroDigit(char c);
        bool isDigit(char c);
        bool lookUpReserved(const string &name, sym::SYMBOL &sy);
};

#endif // LEX_ANALYZER_H
