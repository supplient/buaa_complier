#include <iostream>
#include <string>

#include "LexAnalyzer.h"
#include "GrammarAnalyzer.h"
#include "log.h"
#include "ConstVarRemover.h"
#include "Backend.h"

using namespace std;

void backTest(string filename){
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
    vector<FuncTuple*> func_tuples = program->dumpFunc(tab);

    ConstVarRemover::work(tab, func_tuples);

    cout << "Start dump name table." << endl;
    cout << "---------------------------" << endl;
    cout << tab.toString();
    cout << "---------------------------" << endl;
    cout << "Dump done." << endl;

    cout << endl;

    cout << "Start dump tuples." << endl;
    cout << "---------------------------" << endl;
    for(FuncTuple* func_tuple : func_tuples){
        cout << func_tuple->toString() << endl;
    }
    cout << "---------------------------" << endl;
    cout << "Dump done." << endl;

    Backend backend;
    vector<DataCmd*> data_cmds;
    vector<InstCmd*> inst_cmds;
    backend.trans(tab, func_tuples, &data_cmds, &inst_cmds);

    mylog::ass << ".data" << "\n";
    for(DataCmd *cmd: data_cmds){
        mylog::ass << cmd->toString() << "\n";
    }

    mylog::ass << "\n";

    mylog::ass << ".text" << "\n";
    for(InstCmd *cmd: inst_cmds){
        mylog::ass << cmd->toString() << "\n";
    }

    // Release memory
    delete program;
    for(auto func_tuple: func_tuples)
        delete func_tuple;
}
