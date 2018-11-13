#ifndef GRAMMAR_ANALYZER_H
#define GRAMMAR_ANALYZER_H

#include <string>
#include <set>

#include "LexAnalyzer.h"
#include "Program.h"
#include "MainFunc.h"

using namespace std;

class GrammarAnalyzer{
    private:
        typedef set<sym::SYMBOL> SymSet;
    public:
        GrammarAnalyzer(LexAnalyzer &lex):lex(lex){}

        Program* constructProgram();
    private:
        LexAnalyzer &lex;

        void errorRepo(string reason);
        void errorRepo(string reason, int line, int column);
        void fatalRepo(string reason);
        void skip(sym::SYMBOL valid_sym, const SymSet &delimiter);
        void skip(const SymSet &valid_set, const SymSet &delimiter);

        MainFunc* constructMainFunc(const SymSet &);
};

#endif//GRAMMAR_ANALYZER_H