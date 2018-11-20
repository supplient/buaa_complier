#include "GrammarAnalyzer.h"

#include "symbol.h"

using namespace std;

Factor* GrammarAnalyzer::constructFactor(const SymSet &delimiter){
    Factor *factor = NULL;
    int start_line = lex.getLineNo();

    // const_factor
    if(*lex == sym::PLUS
        || *lex == sym::MINUS
        || *lex == sym::UNSIGNED_INTEGER
        || *lex == sym::CHARACTER){
        const string ehd = "const_factor: ";
        SymSet idel = delimiter;
        idel.insert(sym::PLUS);
        idel.insert(sym::MINUS);
        idel.insert(sym::UNSIGNED_INTEGER);
        idel.insert(sym::CHARACTER);

        ConstFactor *const_factor = new ConstFactor();

        if(*lex == sym::CHARACTER){
            // char const
            const_factor->is_char = true;
            const_factor->char_value = lex.getCharValue();
            lex.nextSymbol();
        }
        else{
            // int const
            const_factor->is_char = false;
            const_factor->int_value = constructInteger(idel);
            if(const_factor->int_value == NULL){
                delete const_factor;
                const_factor = NULL;
            }
        }

        if(const_factor != NULL)
            factor = static_cast<Factor*>(const_factor);

        #if HW
        if(const_factor)
            log::hw << "const_factor";
        #endif// HW
    }
    // exp_factor
    else if(*lex == sym::LEFT_ROUND){
        const string ehd = "exp_factor";
        SymSet idel = delimiter;
        idel.insert(sym::LEFT_ROUND);
        // TODO insert exp's head
        idel.insert(sym::RIGHT_ROUND);

        ExpFactor *exp_factor = new ExpFactor();

        lex.nextSymbol();
        exp_factor->exp = constructExpression(idel);
        if(exp_factor->exp == NULL){
            delete exp_factor;
            exp_factor = NULL;
        }

        if(*lex != sym::RIGHT_ROUND){
            errorRepo(ehd + "should be wrapped with ()");
            skip(idel);
        }
        if(*lex == sym::RIGHT_ROUND)
            lex.nextSymbol();

        if(exp_factor)
            factor = static_cast<Factor*>(exp_factor);
        
        #if HW
        log::hw << "exp_factor";
        #endif// HW
    }
    // var_factor || func_factor
    else if(*lex == sym::IDENTIFIER){
        sym::SYMBOL tmp_sym = lex.nextSymbol();
        lex.goBack();

        // func_factor
        if(tmp_sym == sym::LEFT_ROUND){
            FuncFactor *func_factor = new FuncFactor();
            func_factor->call_exp = constructFuncCallExp(delimiter);
            if(func_factor->call_exp == NULL){
                delete func_factor;
                func_factor = NULL;
            }

            if(func_factor)
                factor = static_cast<Factor*>(func_factor);
            
            #if HW
            log::hw << "func_factor";
            #endif//HW
        }
        // var_factor
        else{
            const string ehd = "var_factor";
            SymSet idel = delimiter;
            idel.insert(sym::IDENTIFIER);
            idel.insert(sym::LEFT_SQUARE);
            // TODO insert exp's head
            idel.insert(sym::RIGHT_SQUARE);

            VarFactor *var_factor = new VarFactor();
            var_factor->ident = lex.getStringValue();
            
            lex.nextSymbol();
            if(*lex == sym::LEFT_SQUARE){
                // is array
                var_factor->is_array = true;

                lex.nextSymbol();
                var_factor->select = constructExpression(idel);
                if(var_factor->select == NULL){
                    delete var_factor;
                    var_factor = NULL;
                }

                if(*lex != sym::RIGHT_SQUARE){
                    errorRepo(ehd + "the array selector should be wrapped with []");
                    skip(idel);
                }
                if(*lex == sym::RIGHT_SQUARE)
                    lex.nextSymbol();
            }
            else
                var_factor->is_array = false;

            if(var_factor)
                factor = static_cast<Factor*>(var_factor);

            #if HW
            log::hw << "var_factor";
            #endif// HW
        }
    }

    int end_line = lex.getLineNo();
    if(factor){
        factor->start_line = start_line;
        factor->end_line = end_line;
    }

    return factor;
}

Item* GrammarAnalyzer::constructItem(const SymSet &delimiter){
    const string ehd = "item: ";
    SymSet idel = delimiter;
    // TODO insert Factor's head
    idel.insert(sym::MULTI);
    idel.insert(sym::DIV);
    int start_line = lex.getLineNo();

    Item *item = new Item();

    if(*lex == sym::MULTI || *lex == sym::DIV){
        errorRepo(ehd + "cannnot start with *, /");
        lex.nextSymbol();
    }

    do{
        if(*lex == sym::MULTI || *lex == sym::DIV){
            item->op_list.push_back(*lex);
            lex.nextSymbol();
        }

        Factor *factor = constructFactor(idel);
        if(factor != NULL)
            item->factor_list.push_back(factor);
        // TODO consider to jump a op, if factor construct fail.
    }while(*lex == sym::MULTI || *lex == sym::DIV);

    if(item->factor_list.size() < 1){
        delete item;
        item = NULL;
    }

    int end_line = lex.getLineNo();
    if(item){
        item->start_line = start_line;
        item->end_line = end_line;
    }

#if HW
    if(item)
        log::hw << "item";
#endif// HW

    return item;
}

Expression* GrammarAnalyzer::constructExpression(const SymSet &delimter){
    const string ehd = "expression: ";
    SymSet idel = delimter;
    idel.insert(sym::PLUS);
    idel.insert(sym::MINUS);
    // TODO insert item's head
    int start_line = lex.getLineNo();

    Expression *exp = new Expression();

    if(*lex == sym::PLUS || *lex == sym::MINUS){
        exp->minus = *lex == sym::MINUS;
        lex.nextSymbol();
    }

    Item *first_item = constructItem(idel);
    if(first_item != NULL)
        exp->item_list.push_back(first_item);

    while(*lex == sym::PLUS || *lex == sym::MINUS){
        exp->op_list.push_back(*lex);
        lex.nextSymbol();

        Item *item = constructItem(idel);
        if(item != NULL)
            exp->item_list.push_back(item);
        // TODO consider jump a op if item construct fail
    }

    if(exp->item_list.size() < 1){
        delete exp;
        exp = NULL;
    }

    int end_line = lex.getLineNo();
    if(exp){
        exp->start_line = start_line;
        exp->end_line = end_line;
    }

#if HW
    if(exp)
        log::hw << "expression";
#endif// HW

    return exp;
}
