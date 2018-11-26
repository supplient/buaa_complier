#include "GrammarAnalyzer.h"

#include "symbol.h"
#include "log.h"

using namespace std;

ConstDefine* GrammarAnalyzer::constructConstDefine(const SymSet &delimiter){
    const string ehd = "const_define: ";
    bool loop_for_error_skip = false;
    int start_line = lex.getLineNo();

    ConstDefine *const_define = NULL;
    do{
        // Int const define
        if(*lex == sym::INT){
            mylog::debug << "<int_const_define> once.";

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
            mylog::debug << "<char_const_define> once.";

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

    int end_line = lex.getLineNo();
    if(const_define){
        const_define->start_line = start_line;
        const_define->end_line = end_line;
    }

    #if HW
    if(const_define != NULL)
        mylog::hw << "const define";
    #endif//HW

    return const_define;
}

ConstDecl* GrammarAnalyzer::constructConstDecl(const SymSet &delimiter){
    const string ehd = "const_decl: ";
    int start_line = lex.getLineNo();

    SymSet idel = delimiter;
    idel.insert(sym::CONST);
    // TODO Insert <const_define> 's head
    idel.insert(sym::SEMICOLON);

    ConstDecl *const_decl = new ConstDecl();
    do{
        mylog::debug << "<const_decl> once.";

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

    int end_line = lex.getLineNo();
    if(const_decl){
        const_decl->start_line = start_line;
        const_decl->end_line = end_line;
    }

    #if HW
    if(const_decl != NULL)
        mylog::hw << "const decl";
    #endif//HW

    return const_decl;
}


VarDefine* GrammarAnalyzer::constructVarDefine(const SymSet &delimiter){
    const string ehd = "var_define: ";
    SymSet idel;
    idel.insert(sym::INT);
    idel.insert(sym::CHAR);
    idel.insert(sym::IDENTIFIER);
    idel.insert(sym::LEFT_SQUARE);
    idel.insert(sym::UNSIGNED_INTEGER);
    idel.insert(sym::RIGHT_SQUARE);
    idel.insert(sym::COMMA);
    int start_line = lex.getLineNo();

    VarDefine *var_define = new VarDefine();
    if(*lex != sym::INT && *lex != sym::CHAR){
        errorRepo(ehd + "type can only be char or int.");
        skip(idel, delimiter);
    }
    if(*lex == sym::INT || *lex == sym::CHAR){
        var_define->type = *lex;
    }
    idel.erase(sym::INT);
    idel.erase(sym::CHAR);

    do{
        mylog::debug << "<var_define> once.";
        bool fail_flag = false;
        string ident;
        int dim=0;

        lex.nextSymbol();
        if(*lex != sym::IDENTIFIER){
            errorRepo(ehd + "must assign a identifier as name.");
            skip(idel, delimiter);
        }
        if(*lex == sym::IDENTIFIER){
            ident = lex.getStringValue();
            lex.nextSymbol();
        }
        else
            fail_flag = true;

        if(*lex == sym::LEFT_SQUARE){
            // array
            mylog::debug << "array";
            lex.nextSymbol();
            if(*lex != sym::UNSIGNED_INTEGER){
                errorRepo(ehd + "array's dim must be an unsigned int.");
                skip(idel, delimiter);
            }
            if(*lex == sym::UNSIGNED_INTEGER){
                dim = lex.getIntValue();
                lex.nextSymbol();
            }
            else
                fail_flag = true;

            if(*lex != sym::RIGHT_SQUARE){
                errorRepo(ehd + "array's dim must be braced by []");
                skip(idel, delimiter);
            }
            if(*lex == sym::RIGHT_SQUARE)
                lex.nextSymbol();
            else
                fail_flag = true;
        }

        if(!fail_flag){
            mylog::debug << "var: " + ident + " with dim[" + std::to_string(dim) + "]";
            var_define->ident_list.push_back(ident);
            var_define->dim_list.push_back(dim);
        }
    }while(*lex == sym::COMMA);

    if(var_define->ident_list.size() < 1){
        delete var_define;
        var_define = NULL;
    }

    int end_line = lex.getLineNo();
    if(var_define){
        var_define->start_line = start_line;
        var_define->end_line = end_line;
    }

    #if HW
    if(var_define != NULL)
        mylog::hw << "var define";
    #endif//HW

    return var_define;
}


VarDecl* GrammarAnalyzer::constructVarDecl(bool overlook, const SymSet &delimiter){
    const string ehd = "var_decl: ";
    int start_line = lex.getLineNo();

    SymSet idel = delimiter;
    // TODO insert <var_define> 's head
    idel.insert(sym::SEMICOLON);

    VarDecl *var_decl = new VarDecl();
    do{
        if(overlook){
            // overlook to check whether func_with_return_define
            lex.nextSymbol(); // we assume this is a `Identifier`, will check later
            sym::SYMBOL tmp_sym = lex.nextSymbol();
            lex.goBack();
            lex.goBack();
            if(tmp_sym == sym::LEFT_ROUND){
                // is func_with_return_define
                break;
            }
        }

        // is var_decl
        mylog::debug << "<var_decl> once.";

        VarDefine *var_define = constructVarDefine(idel);
        if(var_define != NULL){
            var_decl->var_define_list.push_back(var_define);
        }

        if(*lex != sym::SEMICOLON){
            errorRepo(ehd + "should end with ;");
            skip(idel, delimiter);
        }
        if(*lex == sym::SEMICOLON)
            lex.nextSymbol();
    }while(*lex == sym::INT || *lex == sym::CHAR);

    if(var_decl->var_define_list.size() < 1){
        // No var define succeed
        delete var_decl;
        var_decl = NULL;
    }

    int end_line = lex.getLineNo();
    if(var_decl){
        var_decl->start_line = start_line;
        var_decl->end_line = end_line;
    }

    #if HW
    if(var_decl != NULL)
        mylog::hw << "var decl";
    #endif//HW

    return var_decl;
}
