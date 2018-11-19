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
    const string ehd = "switch statement: ";
    SymSet idel = delimiter;
    idel.insert(sym::SWITCH);
    idel.insert(sym::LEFT_ROUND);
    // TODO insert expression 's head
    idel.insert(sym::RIGHT_ROUND);
    idel.insert(sym::LEFT_BRACE);
    // TODO insert switch_case_list & switch_default 's head
    idel.insert(sym::RIGHT_BRACE);
    bool fail_flag = false;

    SwitchStatement *switch_state = new SwitchStatement();

    if(*lex != sym::SWITCH){
        errorRepo(ehd + "must start with switch.");
        skip(idel);
    }
    if(*lex == sym::SWITCH)
        lex.nextSymbol();
    else
        fail_flag = true;

    if(*lex != sym::LEFT_ROUND){
        errorRepo(ehd + "must have a switch value.");
        skip(idel);
    }
    if(*lex == sym::LEFT_ROUND)
        lex.nextSymbol();
    else
        fail_flag = true;

    switch_state->exp = constructExpression(idel);
    if(switch_state->exp == NULL)
        fail_flag = true;

    if(*lex != sym::RIGHT_ROUND){
        errorRepo(ehd + "switch value must be wrapped with ().");
        skip(idel);
    }
    if(*lex == sym::RIGHT_ROUND)
        lex.nextSymbol();
    else
        fail_flag = true;

    if(*lex != sym::LEFT_BRACE){
        errorRepo(ehd + "switch case list must be wrapped by {}");
        skip(idel);
    }
    if(*lex == sym::LEFT_BRACE)
        lex.nextSymbol();
    else
        fail_flag = true;

    switch_state->case_list = constructSwitchCaseList(idel);
    if(switch_state->case_list == NULL)
        fail_flag = true;

    if(*lex != sym::RIGHT_BRACE){
        // have default
        if(*lex != sym::DEFAULT){
            errorRepo(ehd + "default statement must start with `default`");
            skip(idel);
        }
        if(*lex == sym::DEFAULT)
            lex.nextSymbol();
        else 
            fail_flag = true;

        if(*lex != sym::COLON){
            errorRepo(ehd + "default must followed by `:`");
            skip(idel);
        }
        if(*lex == sym::COLON)
            lex.nextSymbol();
        else
            fail_flag = true;

        switch_state->default_state = constructStatement(idel);
        if(switch_state->default_state == NULL)
            fail_flag = true;
    }
    else{
        // do not have default
        lex.nextSymbol();
    }

    if(fail_flag){
        delete switch_state;
        switch_state = NULL;
    }

    #if HW
    if(switch_state)
        log::hw << "switch_statement";
    #endif// HW

    return NULL;
}

SwitchCase* GrammarAnalyzer::constructSwitchCase(const SymSet &delimiter){
    const string ehd = "switch case: ";
    SymSet idel = delimiter;
    idel.insert(sym::CASE);
    // TODO insert Integer 's head
    idel.insert(sym::COLON);
    idel.insert(sym::CHARACTER);
    // TODO insert statment 's head
    bool fail_flag = false;

    SwitchCase *switch_case = new SwitchCase();

    if(*lex != sym::CASE){
        errorRepo(ehd + "must start with `case`");
        skip(idel);
    }
    if(*lex == sym::CASE)
        lex.nextSymbol();
    else
        fail_flag = true;

    if(*lex == sym::CHARACTER){
        // char const
        switch_case->is_int_const = false;
        switch_case->char_value = lex.getCharValue();
        lex.nextSymbol();
    }
    else{
        // int const
        switch_case->is_int_const = true;
        switch_case->int_value = constructInteger(idel);
        if(switch_case->int_value == NULL)
            fail_flag = true;
    }

    if(*lex != sym::COLON){
        errorRepo(ehd + "must separate const and statement with `:`");
        skip(idel);
    }
    if(*lex == sym::COLON)
        lex.nextSymbol();
    else
        fail_flag = true;

    switch_case->state = constructStatement(idel);
    if(switch_case->state == NULL)
        fail_flag = true;

    if(fail_flag){
        delete switch_case;
        switch_case = NULL;
    }

    #if HW
    if(switch_case)
        log::hw << "switch_case";
    #endif//HW

    return switch_case;
}

SwitchCaseList* GrammarAnalyzer::constructSwitchCaseList(const SymSet &delimiter){
    SwitchCaseList *case_list = new SwitchCaseList();

    while(*lex == sym::CASE){
        SwitchCase *switch_case = constructSwitchCase(delimiter);
        if(switch_case != NULL)
            case_list->case_list.push_back(switch_case);
    }

    if(case_list->case_list.size() < 1){
        delete case_list;
        case_list = NULL;
    }

    #if HW
    if(case_list)
        log::hw << "switch_case_list";
    #endif// HW

    return case_list;
}