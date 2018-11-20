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
        LexAnalyzer(ifstream& file);
        virtual ~LexAnalyzer();

        sym::SYMBOL nextSymbol();
        bool goBack();

        int getLineNo()const{
            if(history.size() < 1)
                return 1;
            return history.top().line;
        }
        int getColumnNo()const{
            if(history.size() < 1)
                return 0;
            return history.top().column;
        }

        sym::SYMBOL getSymbol(){return symbol;}
        int getIntValue()const{return int_value;}
        char getCharValue()const{return char_value;}
        string getStringValue()const{return string_value;}

        // Wrapper function
        sym::SYMBOL operator*(){return getSymbol();}

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
        sym::SYMBOL symbol;
        unsigned int int_value;
        char char_value;
        string string_value;

        // Char state
        char ch;
        unsigned int line;
        unsigned int column;
        ifstream& file;

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
