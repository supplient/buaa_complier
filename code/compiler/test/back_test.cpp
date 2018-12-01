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
    if(program == NULL || gra.getErrorCount() > 0){
        cerr << "Grammar analyzer failed." << endl;
        exit(-1);
    }
    file.close();

    NameTable tab;
    vector<FuncTuple*> func_tuples = program->dumpFunc(tab);

    if(Element::error_count > 0){
        cerr << "Semantics analyzer failed." << endl;
        exit(-1);
    }

    ConstVarRemover::work(tab, func_tuples);

    mylog::tup << "Start dump name table." << "\n";
    mylog::tup << "---------------------------" << "\n";
    mylog::tup << tab.toString();
    mylog::tup << "---------------------------" << "\n";
    mylog::tup << "Dump done." << "\n";

    mylog::tup << "\n";

    mylog::tup << "Start dump tuples." << "\n";
    mylog::tup << "---------------------------" << "\n";
    for(FuncTuple* func_tuple : func_tuples){
        mylog::tup << func_tuple->toString() << "\n";
    }
    mylog::tup << "---------------------------" << "\n";
    mylog::tup << "Dump done." << "\n";

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
