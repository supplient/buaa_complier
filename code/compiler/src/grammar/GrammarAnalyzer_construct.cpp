#include "GrammarAnalyzer.h"

#include <iostream>

#include "symbol.h"

using namespace std;

Program* GrammarAnalyzer::constructProgram(){
    string ehd = "program: ";
    SymSet idel;
    idel.insert(sym::ENDOFFILE);

    Program *program = new Program();
    lex.nextSymbol();

    // TODO add const_decl
    // TODO add var_decl
    // TODO add func_define

    program->main_func = constructMainFunc(idel);
    if(program->main_func == NULL){
        errorRepo(ehd + "cannot find main function.");
        delete program;
        return NULL;
    }

    return program;
}

MainFunc* GrammarAnalyzer::constructMainFunc(const SymSet &delimiter){
    string ehd = "main_func: ";// error head
    SymSet idel;// internal delimiter -- just a temp SymSet for skip

    MainFunc* main_func = new MainFunc();
    if(*lex != sym::VOID){
        errorRepo(ehd + "should return `void`.");
        idel = delimiter;
        idel.insert(sym::LEFT_ROUND);
        idel.insert(sym::LEFT_BRACE);
        skip(sym::VOID, idel);
        if(*lex != sym::VOID){
            delete main_func;
            return NULL;
        }
    }
    lex.nextSymbol();
    if(*lex != sym::MAIN){
        errorRepo(ehd + "function name should be `main`.");
        idel = delimiter;
        idel.insert(sym::LEFT_ROUND);
        idel.insert(sym::LEFT_BRACE);
        skip(sym::MAIN, idel);
        if(*lex != sym::MAIN){
            delete main_func;
            return NULL;
        }
    }
    lex.nextSymbol();

    return main_func;
}