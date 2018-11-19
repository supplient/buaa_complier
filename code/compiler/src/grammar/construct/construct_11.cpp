#include "GrammarAnalyzer.h"

#include "symbol.h"

using namespace std;

Program* GrammarAnalyzer::constructProgram(){
    const string ehd = "program: ";
    SymSet idel;
    idel.insert(sym::ENDOFFILE);

    Program *program = new Program();
    lex.nextSymbol();

    // const_decl
    if(*lex == sym::CONST){
        SymSet const_decl_del = idel;
        // TODO insert var_decl 's head
        // TODO insert func_define 's head
        const_decl_del.insert(sym::VOID); // main_func 's head
        program->const_decl = constructConstDecl(const_decl_del);
    }
    else
        program->const_decl = NULL;

    // var_decl
    if(*lex == sym::INT || *lex == sym::CHAR){
        SymSet var_decl_del = idel;
        // TODO insert func_define 's head
        var_decl_del.insert(sym::VOID); // main_func 's head
        program->var_decl = constructVarDecl(true, var_decl_del);
        // [Note] var_decl can have a suffix with `int|char identifier(`
        // When meet this, just stop var_decl, it's valid.
    }
    else
        program->var_decl = NULL;


    do{
        // check whether main_func
        sym::SYMBOL tmp_return_type = *lex;
        sym::SYMBOL tmp_func_name = lex.nextSymbol();
        lex.goBack();
        if(tmp_return_type == sym::VOID
            && tmp_func_name == sym::MAIN)
            break;

        FuncDefine *func_define = constructFuncDefine(idel);
        if(func_define)
            program->func_define_list.push_back(func_define);
    }while(*lex != sym::ENDOFFILE);

    program->main_func = constructMainFunc(idel);
    if(program->main_func == NULL){
        errorRepo(ehd + "cannot find main function.");
        delete program;
        program = NULL;
    }

    if(*lex != sym::ENDOFFILE){
        errorRepo(ehd + "nothing should occur after main_func");
        skip(idel);
    }

#if HW
    if(program != NULL)
        log::hw << "program";
#endif//HW

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

#if HW
    if(main_func != NULL)
        log::hw << "main function";
#endif//HW

    return main_func;
}
