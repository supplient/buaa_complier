#ifndef GRAMMAR_ANALYZER_H
#define GRAMMAR_ANALYZER_H

#include <string>
#include <set>

#include "LexAnalyzer.h"

//2
#include "Integer.h"

//4
#include "ConstDecl.h"
#include "ConstDefine.h"

//10
#include "CompoundStatement.h"

//11
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

        // 2
        Integer* constructInteger(const SymSet &);

        // 4
        ConstDefine* constructConstDefine(const SymSet &);
        ConstDecl* constructConstDecl(const SymSet &);
        VarDefine* constructVarDefine(const SymSet &);
        VarDecl* constructVarDecl(const SymSet &);
        // 10
        CompoundStatement* constructCompoundStatement(const SymSet &suffix, const SymSet &delimiter);
        // 11
        MainFunc* constructMainFunc(const SymSet &);
};

#endif//GRAMMAR_ANALYZER_H