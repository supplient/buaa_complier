#include "GrammarAnalyzer.h"

#include "symbol.h"

using namespace std;

Condition* GrammarAnalyzer::constructCondition(const SymSet &delimiter){
    const string ehd = "condition: ";
    SymSet idel = delimiter;
    idel.insert(sym::LESS);
    idel.insert(sym::LESSOREQUAL);
    idel.insert(sym::MORE);
    idel.insert(sym::MOREOREQUAL);
    idel.insert(sym::NOTEQUAL);
    idel.insert(sym::EQUAL);

    Condition *cond = new Condition();

    cond->left_exp = constructExpression(idel);

    switch(*lex){
        case sym::LESS:
        case sym::LESSOREQUAL:
        case sym::MORE:
        case sym::MOREOREQUAL:
        case sym::NOTEQUAL:
        case sym::EQUAL:
            cond->has_comp = true;
            cond->op = *lex;
            lex.nextSymbol();
            cond->right_exp = constructExpression(idel);
            break;
        default:
            cond->has_comp = false;
            cond->right_exp = NULL;
    }

    if(cond->left_exp == NULL
        || (cond->has_comp && cond->right_exp == NULL)){
        // fail
        delete cond;
        cond = NULL;
    }

    #if HW
    if(cond)
        log::hw << "condition";
    #endif// HW

    return cond;
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
    #endif// HW

    return if_state;
}

WhileStatement* GrammarAnalyzer::constructWhileStatement(const SymSet &delimiter){
    const string ehd = "while statement: ";
    SymSet idel =delimiter;
    idel.insert(sym::WHILE);
    idel.insert(sym::LEFT_ROUND);
    // TODO insert condition 's head
    idel.insert(sym::RIGHT_ROUND);
    // TODO insert statement 's head
    bool fail_flag = false;

    WhileStatement *while_state = new WhileStatement();

    if(*lex != sym::WHILE){
        errorRepo(ehd + "must start with while");
        skip(idel);
    }
    if(*lex == sym::WHILE)
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

    while_state->cond = constructCondition(idel);
    if(while_state->cond == NULL)
        fail_flag = true;

    if(*lex != sym::RIGHT_ROUND){
        errorRepo(ehd + "condition must be wrapped by ()");
        skip(idel);
    }
    if(*lex == sym::RIGHT_ROUND)
        lex.nextSymbol();
    else
        fail_flag = true;

    while_state->state = constructStatement(idel);
    if(while_state->state == NULL)
        fail_flag = true;

    if(fail_flag){
        delete while_state;
        while_state = NULL;
    }

    #if HW
    if(while_state)
        log::hw << "while statement";
    #endif// HW

    return while_state;
}

SwitchStatement* GrammarAnalyzer::constructSwitchStatement(const SymSet &delimiter){
    // TODO
    return NULL;
}
