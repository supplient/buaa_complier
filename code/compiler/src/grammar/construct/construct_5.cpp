#include "GrammarAnalyzer.h"

#include "symbol.h"

using namespace std;

Param* GrammarAnalyzer::constructParam(const SymSet &delimiter){
    const string ehd = "param: ";
    SymSet idel = delimiter;
    idel.insert(sym::INT);
    idel.insert(sym::CHAR);
    idel.insert(sym::IDENTIFIER);
    bool fail_flag = false;
    int start_line = lex.getLineNo();

    Param *param = new Param();

    switch(*lex){
        case sym::INT:
        case sym::CHAR:
            break;
        default:
            errorRepo(ehd + "invaild type.");
            skip(idel);
    }
    switch(*lex){
        case sym::INT:
        case sym::CHAR:
            param->type = *lex;
            lex.nextSymbol();
            break;
        default:
            fail_flag = true;
    }

    if(*lex != sym::IDENTIFIER){
        errorRepo(ehd + "must have a name");
        skip(idel);
    }
    if(*lex == sym::IDENTIFIER){
        param->ident = lex.getStringValue();
        lex.nextSymbol();
    }
    else
        fail_flag = true;
    
    if(fail_flag){
        delete param;
        param = NULL;
    }

    int end_line = lex.getLineNo();
    if(param){
        param->start_line = start_line;
        param->end_line = end_line;
    }

    #if HW
    if(param)
        log::hw << "param";
    #endif//HW

    return param;
}

FuncDefine* GrammarAnalyzer::constructFuncDefine(const SymSet &delimiter){
    const string ehd =  "function define: ";
    SymSet idel = delimiter;
    idel.insert(sym::VOID);
    idel.insert(sym::INT);
    idel.insert(sym::CHAR);
    idel.insert(sym::IDENTIFIER);
    idel.insert(sym::LEFT_ROUND);
    // TODO isnert param 's head
    idel.insert(sym::RIGHT_ROUND);
    idel.insert(sym::LEFT_BRACE);
    // TODO insert compound_statement's head
    idel.insert(sym::RIGHT_BRACE);
    bool fail_flag = false;
    int start_line = lex.getLineNo();

    FuncDefine *func_def = new FuncDefine();

    switch(*lex){
        case sym::VOID:
        case sym::INT:
        case sym::CHAR:
            break;
        default:
            errorRepo(ehd + "must assign a return type.");
            skip(idel);
    }
    switch(*lex){
        case sym::VOID:
        case sym::INT:
        case sym::CHAR:
            func_def->return_type = *lex;
            lex.nextSymbol();
            break;
        default:
            fail_flag = true;
    }

    if(*lex != sym::IDENTIFIER){
        errorRepo(ehd + "must assign a function name.");
        skip(idel);
    }
    if(*lex == sym::IDENTIFIER){
        func_def->func_name = lex.getStringValue();
        lex.nextSymbol();
    }
    else
        fail_flag = true;

    if(*lex != sym::LEFT_ROUND){
        errorRepo(ehd + "must have a param list wrapped with ()");
        skip(idel);
    }
    if(*lex == sym::LEFT_ROUND)
        lex.nextSymbol();
    else
        fail_flag = true;

    if(*lex != sym::RIGHT_ROUND){
        // have param list
        // TODO
        Param *param = constructParam(idel);
        if(param != NULL)
            func_def->param_list.push_back(param);
        while(*lex == sym::COMMA){
            lex.nextSymbol();
            param = constructParam(idel);
            if(param)
                func_def->param_list.push_back(param);
        }

        if(func_def->param_list.size() < 1)
            fail_flag = true;
    }

    if(*lex != sym::RIGHT_ROUND){
        errorRepo(ehd + "param list must be ended with )");
        skip(idel);
    }
    if(*lex == sym::RIGHT_ROUND)
        lex.nextSymbol();
    else
        fail_flag = true;

    if(*lex != sym::LEFT_BRACE){
        errorRepo(ehd + "statements must be wrapped with {}");
        skip(idel);
    }
    if(*lex == sym::LEFT_BRACE)
        lex.nextSymbol();
    else
        fail_flag = true;

    SymSet suffix;
    suffix.insert(sym::RIGHT_BRACE);
    func_def->compound_state = constructCompoundStatement(suffix, idel);
    if(func_def->compound_state == NULL)
        fail_flag = true;

    if(*lex != sym::RIGHT_BRACE){
        errorRepo(ehd + "statements must be wrapped with {}");
        skip(idel);
    }
    if(*lex == sym::RIGHT_BRACE)
        lex.nextSymbol();
    else
        fail_flag = true;

    if(fail_flag){
        delete func_def;
        func_def = NULL;
    }

    int end_line = lex.getLineNo();
    if(func_def){
        func_def->start_line = start_line;
        func_def->end_line = end_line;
    }

    #if HW
    if(func_def)
        log::hw << "function_define";
    #endif//HW

    return func_def;
}
