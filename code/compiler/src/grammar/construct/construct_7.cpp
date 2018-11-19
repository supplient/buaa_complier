#include "GrammarAnalyzer.h"

#include "symbol.h"

using namespace std;

Condition* GrammarAnalyzer::constructCondition(const SymSet &delimiter){
    // TODO
    return NULL;
}

IfStatement* GrammarAnalyzer::constructIfStatement(const SymSet &delimiter){
    const string ehd = "if statement: ";
    SymSet idel = delimiter;
    idel.insert(sym::IF);
    idel.insert(sym::LEFT_ROUND);
    // TODO insert condition's head
    idel.insert(sym::RIGHT_ROUND);
    // TODO insert statement's head
    bool fail_flag = false;

    IfStatement *if_state = new IfStatement();

    if(*lex != sym::IF){
        errorRepo(ehd + "must start with if");
        skip(idel);
    }
    if(*lex == sym::IF)
        lex.nextSymbol();
    else
        fail_flag = true;

    if(*lex != sym::LEFT_ROUND){
        errorRepo(ehd + "must have condition");
        skip(idel);
    }
    if(*lex == sym::LEFT_ROUND)
        lex.nextSymbol();
    else
        fail_flag = true;

    if_state->cond = constructCondition(idel);
    if(if_state->cond == NULL)
        fail_flag = true;

    if(*lex != sym::RIGHT_ROUND){
        errorRepo(ehd + "condition must be wrapped by ()");
        skip(idel);
    }
    if(*lex == sym::RIGHT_ROUND)
        lex.nextSymbol();
    else
        fail_flag = true;

    if_state->state = constructStatement(idel);
    if(if_state->state == NULL)
        fail_flag = true;

    if(fail_flag){
        delete if_state;
        if_state = NULL;
    }

    #if HW
    if(if_state)
        log::hw << "if statement";
    #endif// HE

    return if_state;
}

WhileStatement* GrammarAnalyzer::constructWhileStatement(const SymSet &delimiter){
    // TODO
    return NULL;
}

SwitchStatement* GrammarAnalyzer::constructSwitchStatement(const SymSet &delimiter){
    // TODO
    return NULL;
}
