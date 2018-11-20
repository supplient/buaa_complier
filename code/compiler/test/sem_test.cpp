#include <iostream>
#include <string>

#include "LexAnalyzer.h"
#include "GrammarAnalyzer.h"
#include "log.h"

using namespace std;

void semTest(string filename){
    ifstream file(filename, ios_base::binary);
    if(!file.is_open() || !file){
        cerr << "File " << filename << " open failed." << endl;
        throw "File open failed.";
    }
    LexAnalyzer lex(file);
    GrammarAnalyzer gra(lex);

    Program* program = gra.constructProgram();
    if(program == NULL){
        cerr << "Build program failed." << endl;
        exit(-1);
    }
    file.close();
    
    NameTable tab;
    vector<Tuple*> tuples = program->dump(tab);

    cout << "Start dump name table." << endl;
    cout << "---------------------------" << endl;
    cout << tab.toString();
    cout << "---------------------------" << endl;
    cout << "Dump done." << endl;

    cout << endl;

    cout << "Start dump tuples." << endl;
    cout << "---------------------------" << endl;
    for(Tuple* tuple : tuples){
        cout << tuple->toString() << endl;
        delete tuple;
    }
    cout << "---------------------------" << endl;
    cout << "Dump done." << endl;
}
