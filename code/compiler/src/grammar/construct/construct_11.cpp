#include "GrammarAnalyzer.h"

#include "symbol.h"

using namespace std;

Program* GrammarAnalyzer::constructProgram(){
    const string ehd = "program: ";
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
    const string ehd = "main_func: ";// error head
    SymSet idel = delimiter;// internal delimiter -- just a temp SymSet for skip
    sym::SYMBOL check_sym;// the symbol to be checked -- just a temp SYMBOL to reduce code.

    // init idel
    idel.insert(sym::VOID);
    idel.insert(sym::MAIN);
    idel.insert(sym::LEFT_ROUND);
    idel.insert(sym::RIGHT_ROUND);
    idel.insert(sym::LEFT_BRACE);
    // TODO insert CompoundStatement 's head
    idel.insert(sym::RIGHT_BRACE);

    MainFunc* main_func = new MainFunc();
    check_sym = sym::VOID;
    if(*lex != check_sym){
        errorRepo(ehd + "should return `void`.");
        skip(idel, delimiter);
        if(*lex == check_sym)
            lex.nextSymbol();
    }
    else
        lex.nextSymbol();
    idel.erase(check_sym);

    check_sym = sym::MAIN;
    if(*lex != check_sym){
        errorRepo(ehd + "function name should be `main`.");
        skip(idel, delimiter);
        if(*lex == check_sym)
            lex.nextSymbol();
    }
    else
        lex.nextSymbol();
    idel.erase(check_sym);

    check_sym = sym::LEFT_ROUND;
    if(*lex != check_sym){
        errorRepo(ehd + "function must have a param list.");
        skip(idel, delimiter);
        if(*lex == check_sym)
            lex.nextSymbol();
    }
    else
        lex.nextSymbol();
    idel.erase(check_sym);

    check_sym = sym::RIGHT_ROUND;
    if(*lex != check_sym){
        errorRepo(ehd + "main function should not have any param.");
        skip(idel, delimiter);
        if(*lex == check_sym)
            lex.nextSymbol();
    }
    else
        lex.nextSymbol();
    idel.erase(check_sym);

    check_sym = sym::LEFT_BRACE;
    if(*lex != check_sym){
        errorRepo(ehd + "function's statements mush be wrapped by {}.");
        skip(idel, delimiter);
        if(*lex == check_sym)
            lex.nextSymbol();
    }
    else
        lex.nextSymbol();
    idel.erase(check_sym);

    SymSet suffix;
    suffix.insert(sym::RIGHT_BRACE);
    main_func->compound_statement =
        constructCompoundStatement(suffix, delimiter);
    if(main_func->compound_statement == NULL){
        // Should have errorRepoed
        delete main_func;
        return NULL;
    }
    // Should have nextSymboled
    // TODO erase compound_statement's head from idel
    // Note: Must reserve RIGHT_BRACE, use set's diff to do this.

    check_sym = sym::RIGHT_BRACE;
    if(*lex != check_sym){
        errorRepo(ehd + "function's statements mush be wrapped by {}.");
        skip(idel, delimiter);
        if(*lex == check_sym)
            lex.nextSymbol();
    }
    else
        lex.nextSymbol();
    idel.erase(check_sym);

    return main_func;
}
