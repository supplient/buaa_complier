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
    error_count++;
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
    skip(skip_set);
}

void GrammarAnalyzer::skip(const SymSet &skip_set){
    if(skip_set.find(lex.getSymbol()) != skip_set.end())
        return;
    while(skip_set.find(lex.nextSymbol()) == skip_set.end())
        if(lex.getSymbol() == sym::ENDOFFILE){ // This is only a secure, should never reach.
            fatalRepo("skip: ENDOFFILE not in delimiter set!");
            break;
        }
    return;
}