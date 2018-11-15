#include "GrammarAnalyzer.h"

#include "symbol.h"
#include "log.h"

using namespace std;

ConstDefine* GrammarAnalyzer::constructConstDefine(const SymSet &delimiter){
    // TODO
    return NULL;
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
