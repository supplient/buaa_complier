#include "GrammarAnalyzer.h"

#include <iostream>

#include "symbol.h"

using namespace std;

void GrammarAnalyzer::errorRepo(string reason){
    errorRepo(reason, lex.getLineNo(), lex.getColumnNo());
}

void GrammarAnalyzer::errorRepo(string reason, int line, int column){
    // TODO print origin code line.
    cerr << "Grammar Error at line[" << line << "] column[" << column << "]--" << endl;
    cerr << "\t" << reason << endl;
}

void GrammarAnalyzer::fatalRepo(string reason){
    cerr << "Grammar Fatal!-- " << endl << "\t" << reason << endl;
    exit(-1);
}

void GrammarAnalyzer::skip(sym::SYMBOL valid_sym, const SymSet &delimiter){
    SymSet valid_set;
    valid_set.insert(valid_sym);
    skip(valid_set, delimiter);
}

void GrammarAnalyzer::skip(const SymSet &valid_set, const SymSet &delimiter){
    SymSet skip_set = valid_set;
    skip_set.insert(delimiter.begin(), delimiter.end()); // Merge
    if(skip_set.find(lex.getSymbol()) != skip_set.end())
        return;
    while(skip_set.find(lex.nextSymbol()) == skip_set.end())
        if(lex.getSymbol() == sym::ENDOFFILE){ // This is only a secure, should never reach.
            fatalRepo("skip: ENDOFFILE not in delimiter set!");
            break;
        }
    return;
}

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
