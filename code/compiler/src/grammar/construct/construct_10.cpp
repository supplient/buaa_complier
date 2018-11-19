#include "GrammarAnalyzer.h"

#include "symbol.h"

using namespace std;

Statement* GrammarAnalyzer::constructStatement(const SymSet &delimiter){
    Statement *state = NULL;

    // input_statement
    if(*lex == sym::SCANF){
        const string ehd = "input_statement: ";
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
        if(input_state)
            log::hw << "input statement";
        #endif//HW
    }
    // output_statement
    else if(*lex == sym::PRINTF){
        const string ehd = "output_statement: ";
        SymSet idel = delimiter;
        idel.insert(sym::LEFT_ROUND);
        idel.insert(sym::STRING);
        // TODO insert expression 's head
        idel.insert(sym::RIGHT_ROUND);

        OutputStatement *output_state = new OutputStatement();

        lex.nextSymbol();
        if(*lex != sym::LEFT_ROUND){
            errorRepo(ehd + "should be wrapped with ()");
            skip(idel);
        }
        if(*lex == sym::LEFT_ROUND)
            lex.nextSymbol();

        // If start with string
        if(*lex == sym::STRING){
            output_state->has_string = true;
            output_state->str_value = lex.getStringValue();
            lex.nextSymbol();

            // check whether followed by an expression
            if(*lex == sym::COMMA)
                lex.nextSymbol();
        }

        output_state->exp_value = constructExpression(idel);

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

        if(!output_state->has_string && output_state->exp_value==NULL){
            delete output_state;
            output_state = NULL;
        }
        else
            state = static_cast<Statement*>(output_state);

        #if HW
        if(output_state)
            log::hw << "output statement";
        #endif//HW
    }
    // return_statement
    else if(*lex == sym::RETURN){
        const string ehd = "return_statement: ";
        SymSet idel = delimiter;
        idel.insert(sym::RETURN);
        idel.insert(sym::LEFT_ROUND);
        // TODO insert exp's head
        idel.insert(sym::RIGHT_ROUND);
        idel.insert(sym::SEMICOLON);

        ReturnStatement *return_state = new ReturnStatement();

        lex.nextSymbol();
        if(*lex == sym::LEFT_ROUND){
            // have return value`
            lex.nextSymbol();

            return_state->exp = constructExpression(idel);
            if(return_state->exp == NULL){
                delete return_state;
                return_state = NULL;
            }

            if(*lex != sym::RIGHT_ROUND){
                errorRepo(ehd + "return value shoudl be wrapped with ()");
                skip(idel);
            }
            if(*lex == sym::RIGHT_ROUND)
                lex.nextSymbol();
        }
        else
            return_state->exp = NULL;

        if(*lex != sym::SEMICOLON){
            errorRepo(ehd + "should end with ;");
            skip(idel);
        }
        if(*lex == sym::SEMICOLON)
            lex.nextSymbol();

        if(return_state)
            state = static_cast<Statement*>(return_state);

        #if HW
        log::hw << "return_state";
        #endif// HW
    }
    // assign_statement || func_call_statement
    else if(*lex == sym::IDENTIFIER){
        sym::SYMBOL tmp_sym = lex.nextSymbol();
        lex.goBack();
        // func_call_statement
        if(tmp_sym == sym::LEFT_ROUND){
            // TODO
        }
        // assign_statement
        else{
            const string ehd = "assign_statement: ";
            SymSet idel = delimiter;
            idel.insert(sym::IDENTIFIER);
            idel.insert(sym::LEFT_SQUARE);
            // TODO insert exp's head
            idel.insert(sym::RIGHT_SQUARE);
            idel.insert(sym::ASSIGN);
            bool fail_flag = false;

            AssignStatement *assign_state = new AssignStatement();

            assign_state->ident = lex.getStringValue();
            if(lex.nextSymbol() == sym::LEFT_SQUARE){
                // is array
                assign_state->is_array = true;
                
                lex.nextSymbol();
                assign_state->select = constructExpression(idel);
                if(assign_state->select == NULL)
                    fail_flag = true;

                if(*lex != sym::RIGHT_SQUARE){
                    errorRepo(ehd + "array selector should be wrapped with []");
                    skip(idel);
                }
                if(*lex == sym::RIGHT_SQUARE)
                    lex.nextSymbol();
            }
            else
                assign_state->is_array = false;

            if(*lex != sym::ASSIGN){
                errorRepo(ehd + "must have an assign op");
                skip(idel);
            }
            if(*lex == sym::ASSIGN)
                lex.nextSymbol();

            assign_state->exp = constructExpression(idel);
            if(assign_state->exp == NULL)
                fail_flag = true;

            if(fail_flag){
                delete assign_state;
                assign_state = NULL;
            }

            if(*lex != sym::SEMICOLON){
                errorRepo(ehd + "must end with ;");
                skip(idel);
            }
            if(*lex == sym::SEMICOLON)
                lex.nextSymbol();

            if(assign_state)
                state = static_cast<Statement*>(assign_state);

            #if HW
            log::hw << "assign_statement";
            #endif// HW
        }
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
