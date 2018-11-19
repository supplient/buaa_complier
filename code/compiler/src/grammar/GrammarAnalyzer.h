#ifndef GRAMMAR_ANALYZER_H
#define GRAMMAR_ANALYZER_H

#include <string>
#include <set>
#include <iostream>

#include "LexAnalyzer.h"
#include "log.h"

#define HW true

//2
#include "Integer.h"

//3
#include "Expression.h"

//4
#include "ConstDecl.h"
#include "ConstDefine.h"

//5
#include "Param.h"
#include "FuncDefine.h"

//7
#include "Condition.h"
#include "SwitchCase.h"
#include "SwitchCaseList.h"

//8
#include "FuncCallExp.h"

//10
#include "Statement.h"
#include "StatementList.h"
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
        void skip(const SymSet &delimiter);

        // 2
        Integer* constructInteger(const SymSet &);

        // 3
        Factor* constructFactor(const SymSet &);
        Item* constructItem(const SymSet &);
        Expression* constructExpression(const SymSet &);

        // 4
        ConstDefine* constructConstDefine(const SymSet &);
        ConstDecl* constructConstDecl(const SymSet &);
        VarDefine* constructVarDefine(const SymSet &);
        VarDecl* constructVarDecl(bool overlook, const SymSet &);

        // 5
        Param* constructParam(const SymSet &);
        FuncDefine* constructFuncDefine(const SymSet &);

        // 7
        Condition* constructCondition(const SymSet &);
        SwitchCase* constructSwitchCase(const SymSet &);
        SwitchCaseList* constructSwitchCaseList(const SymSet &);

        // 8
        FuncCallExp* constructFuncCallExp(const SymSet &);

        // 10
        IfStatement* constructIfStatement(const SymSet &);
        WhileStatement* constructWhileStatement(const SymSet &);
        SwitchStatement* constructSwitchStatement(const SymSet &);
        Statement* constructStatement(const SymSet &);
        StatementList* constructStatementList(const SymSet &suffix, const SymSet &delimiter);
        CompoundStatement* constructCompoundStatement(const SymSet &suffix, const SymSet &delimiter);

        // 11
        MainFunc* constructMainFunc(const SymSet &);
};

#endif//GRAMMAR_ANALYZER_H