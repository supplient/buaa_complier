#include <iostream>
#include <string>

#include "LexAnalyzer.h"
#include "GrammarAnalyzer.h"
#include "log.h"
#include "ConstVarRemover.h"
#include "BasicBlockSplitter.h"
#include "dag/Worker.h"
#include "Backend.h"

using namespace std;

void modiTest(string filename){
    // build input stream
    ifstream file(filename, ios_base::binary);
    if(!file.is_open() || !file){
        cerr << "File " << filename << " open failed." << endl;
        throw "File open failed.";
    }

    // init analyzers
    LexAnalyzer lex(file);
    GrammarAnalyzer gra(lex);

    // grammar analyze
    Program* program = gra.constructProgram();
    if(program == NULL || gra.getErrorCount() > 0){
        cerr << "Grammar analyzer failed." << endl;
        exit(-1);
    }
    file.close();

    // semantics analyze
    NameTable tab;
    vector<FuncTuple*> func_tuples = program->dumpFunc(tab);

    if(Element::error_count > 0){
        cerr << "Semantics analyzer failed." << endl;
        exit(-1);
    }

    // Start modify
    // remove const vars
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

    mylog::tup << "\n";

    if(MODIFY){
        // split basic blocks
        vector<FuncBlock*> func_blocks = BasicBlockSplitter::work(func_tuples);

        // DAG modify
        if(DAG_MODIFY){
            for(FuncBlock *func_block: func_blocks){
                for(BasicBlock *block: func_block->blocks){
                    dag::Worker::work(tab.getFuncNameTable(func_block->func_entry->name), block);
                }
            }
            // TODO free origin func_tuples after DAG modify
        }

        cout << "Start dump basic blocks." << "\n";
        cout << "---------------------------" << "\n";
        for(FuncBlock* func_block : func_blocks){
            cout << func_block->toString() << "\n";
        }
        cout << "---------------------------" << "\n";
        cout << "Dump done." << "\n";

        // dump func_tuples from func_blocks after all modify
        func_tuples.clear();
        for(FuncBlock *func_block: func_blocks)
            func_tuples.push_back(func_block->dumpFuncTuple());

        mylog::tup << "Start dump name table after modify." << "\n";
        mylog::tup << "---------------------------" << "\n";
        mylog::tup << tab.toString();
        mylog::tup << "---------------------------" << "\n";
        mylog::tup << "Dump done." << "\n";

        mylog::tup << "\n";

        mylog::tup << "\nStart dump tuples after modify." << "\n";
        mylog::tup << "---------------------------" << "\n";
        for(FuncTuple* func_tuple : func_tuples){
            mylog::tup << func_tuple->toString() << "\n";
        }
        mylog::tup << "---------------------------" << "\n";
        mylog::tup << "Dump done." << "\n";

        cout << "\n";
    }

    // MIPS backend
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
