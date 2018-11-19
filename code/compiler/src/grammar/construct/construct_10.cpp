#include "GrammarAnalyzer.h"

#include "symbol.h"

using namespace std;

Statement* GrammarAnalyzer::constructStatement(const SymSet &delimiter){
    Statement *state = NULL;

    // input_statement
    if(*lex == sym::SCANF){
        const string ehd = "scanf statement: ";
        SymSet idel = delimiter;
        idel.insert(sym::LEFT_ROUND);
        idel.insert(sym::IDENTIFIER);
        idel.insert(sym::COMMA);
        idel.insert(sym::RIGHT_ROUND);

        InputStatement *input_state = new InputStatement();

        lex.nextSymbol();
        if(*lex != sym::LEFT_ROUND){
            errorRepo(ehd + "should be wrapped with ()");
            skip(idel);
        }
        if(*lex == sym::LEFT_ROUND)
            lex.nextSymbol();

        do{
            if(*lex == sym::COMMA)
                lex.nextSymbol();
            
            if(*lex != sym::IDENTIFIER){
                errorRepo(ehd + "can only input into some variable.");
                skip(idel);
            }
            if(*lex == sym::IDENTIFIER){
                input_state->ident_list.push_back(lex.getStringValue());
                lex.nextSymbol();
            }
        }while(*lex == sym::COMMA);

        if(*lex != sym::RIGHT_ROUND){
            errorRepo(ehd + "should be wrapped with ().");
            skip(idel);
        }
        if(*lex == sym::RIGHT_ROUND)
            lex.nextSymbol();

        if(*lex != sym::SEMICOLON){
            errorRepo(ehd + "should end with ;");
            skip(idel);
        }
        if(*lex == sym::SEMICOLON)
            lex.nextSymbol();

        if(input_state->ident_list.size() < 1){
            delete input_state;
            input_state = NULL;
        }
        else
            state = static_cast<Statement*>(input_state);

        #if HW
        log::hw << "input statement";
        #endif//HW
    }

    return state;
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
