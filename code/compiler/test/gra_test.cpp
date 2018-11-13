#include <iostream>
#include <string>

#include "LexAnalyzer.h"
#include "GrammarAnalyzer.h"
#include "log.h"

using namespace std;

void graTest(string filename){
    ifstream file(filename, ios_base::binary);
    if(!file.is_open() || !file){
        cerr << "File " << filename << " open failed." << endl;
        throw "File open failed.";
    }
    LexAnalyzer lex(file);
    GrammarAnalyzer gra(lex);

    Program* program = gra.constructProgram();
    if(program == NULL)
        cerr << "Build program failed." << endl;
    else{
        if(program->main_func == NULL)
            cerr << "Build main function failed." << endl;
        else
            cout << "Great." << endl;
    }
}
