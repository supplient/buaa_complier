#include "GrammarAnalyzer.h"

#include "symbol.h"
#include "log.h"

using namespace std;

ConstDefine* GrammarAnalyzer::constructConstDefine(const SymSet &delimiter){
    const string ehd = "const_define: ";
    bool loop_for_error_skip = false;
    
    ConstDefine *const_define = NULL;
    do{
        // Int const define
        if(*lex == sym::INT){
            log::debug << "<int_const_define> once.";

            loop_for_error_skip = false;
            IntConstDefine *int_const_define = new IntConstDefine();
            SymSet int_idel = delimiter;
            int_idel.insert(sym::IDENTIFIER);
            int_idel.insert(sym::ASSIGN);
            // TODO insert integer's head
            int_idel.insert(sym::COMMA);

            do{
                bool fail_flag = false;
                string ident;
                Integer *integer;

                lex.nextSymbol();
                if(*lex != sym::IDENTIFIER){
                    errorRepo(ehd + "shoudl assign identifier.");
                    skip(sym::IDENTIFIER, int_idel);
                }
                if(*lex == sym::IDENTIFIER){
                    ident = lex.getStringValue();
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

                integer = constructInteger(int_idel);
                if(integer == NULL)
                    fail_flag = true;

                if(!fail_flag){
                    int_const_define->ident_list.push_back(ident);
                    int_const_define->integer_list.push_back(integer);
                }
            }while(*lex == sym::COMMA);

            if(int_const_define->ident_list.size() < 1){
                delete int_const_define;
                int_const_define = NULL;
            }
            else
                const_define = static_cast<ConstDefine*>(int_const_define);
        }
        // Char const define
        else if(*lex == sym::CHAR){
            log::debug << "<char_const_define> once.";

            loop_for_error_skip = false;
            CharConstDefine *char_const_define = new CharConstDefine();
            SymSet char_idel = delimiter;
            char_idel.insert(sym::IDENTIFIER);
            char_idel.insert(sym::ASSIGN);
            char_idel.insert(sym::CHARACTER);
            char_idel.insert(sym::COMMA);

            do{
                bool fail_flag = false;
                string ident;
                char character;

                lex.nextSymbol();
                if(*lex != sym::IDENTIFIER){
                    errorRepo(ehd + "shoudl assign identifier.");
                    skip(sym::IDENTIFIER, char_idel);
                }
                if(*lex == sym::IDENTIFIER){
                    ident = lex.getStringValue();
                    lex.nextSymbol();
                }
                else
                    fail_flag = true;

                if(*lex != sym::ASSIGN){
                    errorRepo(ehd + "must assign initial value.");
                    skip(sym::EQUAL, char_idel);
                }
                if(*lex == sym::ASSIGN)
                    lex.nextSymbol();
                else
                    fail_flag = true;

                if(*lex != sym::CHARACTER){
                    errorRepo(ehd + "should assign a character for char const.");
                    skip(sym::CHARACTER, char_idel);
                }
                if(*lex == sym::CHARACTER){
                    character = lex.getCharValue();
                    lex.nextSymbol();
                }
                else
                    fail_flag = true;

                if(!fail_flag){
                    char_const_define->ident_list.push_back(ident);
                    char_const_define->char_list.push_back(character);
                }
            }while(*lex == sym::COMMA);

            if(char_const_define->ident_list.size() < 1){
                delete char_const_define;
                char_const_define = NULL;
            }
            else
                const_define = static_cast<ConstDefine*>(char_const_define);
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
