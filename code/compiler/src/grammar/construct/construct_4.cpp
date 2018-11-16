#include "GrammarAnalyzer.h"

#include "symbol.h"
#include "log.h"

using namespace std;

ConstDefine* GrammarAnalyzer::constructConstDefine(const SymSet &delimiter){
    const string ehd = "const_define: ";
    bool loop_for_error_skip = false;
    bool fail_flag = false;
    
    ConstDefine *const_define = NULL;
    do{
        if(*lex == sym::INT){

            loop_for_error_skip = false;
            IntConstDefine *int_const_define = new IntConstDefine();
            SymSet int_idel = delimiter;
            int_idel.insert(sym::COMMA);

            do{
                lex.nextSymbol();
                if(*lex != sym::IDENTIFIER){
                    errorRepo(ehd + "shoudl assign identifier.");
                    skip(sym::IDENTIFIER, int_idel);
                }
                if(*lex == sym::IDENTIFIER){
                    int_const_define->ident = lex.getStringValue();
                    lex.nextSymbol();
                }
                else
                    fail_flag = true;

                if(*lex != sym::ASSIGN){
                    errorRepo(ehd + "must assign initial value.");
                    skip(sym::EQUAL, int_idel);
                }
                if(*lex == sym::ASSIGN)
                    lex.nextSymbol();
                else
                    fail_flag = true;

                int_const_define->integer = constructInteger(int_idel);
                if(int_const_define->integer == NULL)
                    fail_flag = true;
            }while(*lex == sym::COMMA);

            if(fail_flag){
                delete int_const_define;
                int_const_define = NULL;
            }
            else
                const_define = static_cast<ConstDefine*>(int_const_define);
        }
        else if(*lex == sym::CHAR){
            loop_for_error_skip = false;
            // TODO
        }
        else{
            errorRepo(ehd + "should start with `int` or `char`.");
            SymSet idel;
            idel.insert(sym::INT);
            idel.insert(sym::CHAR);
            skip(idel, delimiter);
            if(*lex==sym::INT || *lex==sym::CHAR)
                loop_for_error_skip = true;
            else
                const_define = NULL;
        }
    }while(loop_for_error_skip);

    return const_define;
}

ConstDecl* GrammarAnalyzer::constructConstDecl(const SymSet &delimiter){
    const string ehd = "const_decl: ";

    SymSet idel = delimiter;
    idel.insert(sym::CONST);
    // TODO Insert <const_define> 's head
    idel.insert(sym::SEMICOLON);

    ConstDecl *const_decl = new ConstDecl();
    do{
        log::debug << "<const_decl> once.";

        if(*lex != sym::CONST){
            errorRepo(ehd + "should start with const.");
            skip(idel, delimiter);
            if(*lex == sym::CONST)
                lex.nextSymbol();
        }
        else
            lex.nextSymbol();

        ConstDefine *const_define = constructConstDefine(idel);
        if(const_define != NULL){
            const_decl->const_define_list.push_back(const_define);
        }

        if(*lex != sym::SEMICOLON){
            errorRepo(ehd + "should end with ;.");
            skip(idel, delimiter);
            if(*lex == sym::SEMICOLON)
                lex.nextSymbol();
        }
        else
            lex.nextSymbol();
    }while(*lex == sym::CONST);

    if(const_decl->const_define_list.size() < 1){
        // No const define succeed
        delete const_decl;
        const_decl = NULL;
    }

    return const_decl;
}
