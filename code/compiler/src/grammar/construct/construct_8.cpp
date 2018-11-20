#include "GrammarAnalyzer.h"

#include "symbol.h"

using namespace std;

FuncCallExp* GrammarAnalyzer::constructFuncCallExp(const SymSet &delimiter){
    const string ehd = "func_call_exp: ";
    SymSet idel = delimiter;
    idel.insert(sym::IDENTIFIER);
    idel.insert(sym::LEFT_ROUND);
    // TODO insert value_param_list 's head
    idel.insert(sym::RIGHT_ROUND);
    bool fail_flag = false;
    int start_line = lex.getLineNo();

    FuncCallExp *call_exp = new FuncCallExp();

    if(*lex != sym::IDENTIFIER){
        errorRepo(ehd + "must assign a func's name");
        skip(idel);
    }
    if(*lex == sym::IDENTIFIER){
        call_exp->ident = lex.getStringValue();
        lex.nextSymbol();
    }
    else
        fail_flag = false;

    if(*lex != sym::LEFT_ROUND){
        errorRepo(ehd + "must give a value param list");
        skip(idel);
    }
    if(*lex == sym::LEFT_ROUND)
        lex.nextSymbol();
    else
        fail_flag = false;

    if(*lex != sym::RIGHT_ROUND){
        // have value param
        if(*lex == sym::COMMA)
            errorRepo(ehd + "value param cannot start with ,");

        do{
            if(*lex == sym::COMMA)
                lex.nextSymbol();

            Expression *exp = constructExpression(idel);
            if(exp)
                call_exp->param_list.push_back(exp);
        }while(*lex == sym::COMMA);

        if(call_exp->param_list.size() < 1)
            fail_flag = true;

        if(*lex != sym::RIGHT_ROUND){
            errorRepo(ehd + "value param list must be wrapped with ()");
            skip(idel);
        }
        if(*lex == sym::RIGHT_ROUND)
            lex.nextSymbol();
        else
            fail_flag = true;
    }
    else{
        // have no value param
        lex.nextSymbol();
    }

    if(fail_flag){
        delete call_exp;
        call_exp = NULL;
    }

    int end_line = lex.getLineNo();
    if(call_exp){
        call_exp->start_line = start_line;
        call_exp->end_line = end_line;
    }

    #if HW
    if(call_exp)
        log::hw << "func_call_expression";
    #endif//HW

    return call_exp;
}