#include "GrammarAnalyzer.h"

#include "symbol.h"

using namespace std;

Statement* GrammarAnalyzer::constructStatement(const SymSet &delimiter){
    return NULL;
}

StatementList* GrammarAnalyzer::constructStatementList(const SymSet &suffix, const SymSet &delimiter){
    StatementList *statement_list = new StatementList();
    SymSet idel = delimiter;
    idel.insert(suffix.begin(), suffix.end());

    Statement *state = constructStatement(idel);
    while(state != NULL){
        statement_list->state_list.push_back(state);
        state = constructStatement(idel);
    }

    #if HW
    if(statement_list != NULL)
        log::hw << "statement list";
    #endif//HW

    return statement_list;
}

CompoundStatement* GrammarAnalyzer::constructCompoundStatement(const SymSet &suffix, const SymSet &delimiter){
    CompoundStatement *com_stat = new CompoundStatement();
    SymSet idel = delimiter;
    // TODO insert const_decl, var_decl, statement_list's head
    idel.insert(suffix.begin(), suffix.end());

    // const_decl
    if(*lex == sym::CONST){
        com_stat->const_decl = constructConstDecl(idel);
    }
    else
        com_stat->const_decl = NULL;

    // var_decl
    if(*lex == sym::INT || *lex == sym::CHAR){
        com_stat->var_decl = constructVarDecl(false, idel);
    }
    else
        com_stat->var_decl = NULL;

    // statment_list
    com_stat->statement_list = constructStatementList(suffix, idel);

    #if HW
    if(com_stat != NULL)
        log::hw << "compound statement";
    #endif//HW

    return com_stat;
}
