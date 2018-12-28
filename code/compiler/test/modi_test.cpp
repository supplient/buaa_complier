#include <iostream>
#include <string>

#include "LexAnalyzer.h"
#include "GrammarAnalyzer.h"
#include "log.h"
#include "ConstVarRemover.h"
#include "GlobalRegAllocator.h"
#include "BasicBlockSplitter.h"
#include "dag/Worker.h"
#include "FlowFuncBlock.h"
#include "Backend.h"

using namespace std;

void outputTuples(NameTable &tab, vector<FuncTuple*> func_tuples){
    if(!STO){
        mylog::tup << "Start dump name table." << "\n";
        mylog::tup << "---------------------------" << "\n";
        mylog::tup << tab.toString();
        mylog::tup << "---------------------------" << "\n";
        mylog::tup << "Dump done." << "\n";

        mylog::tup << "\n"; 
    }

    mylog::tup << "Start dump tuples." << "\n";
    mylog::tup << "---------------------------" << "\n";
    if(STO){
        // If STO, we need to output global vars' info by hand.
        FuncNameTable *func_tab = tab.getFuncNameTable(sem::GLOBAL_FUNC_NAME);
        for(const VarEntry *var: func_tab->getUnconstVars()){
            if(var->is_param)
                continue;
            string s = "";
            s += "var ";
            switch(var->type){
                case sym::INT: s += "int"; break;
                case sym::CHAR: s += "char"; break;
                case sym::VOID: s += "void"; break;
                default: throw string("invalid var type: " + to_string(var->type));
            }
            s += " " + var->name + "\n";
            mylog::tup << s;
        }
    }
    for(FuncTuple* func_tuple : func_tuples){
        if(STO)
            mylog::tup << func_tuple->toString(tab) << "\n";
        else
            mylog::tup << func_tuple->toString() << "\n";
    }
    mylog::tup << "---------------------------" << "\n";
    mylog::tup << "Dump done." << "\n";
}

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
    mylog::info << "Doing grammar analyzing...";
    Program* program = gra.constructProgram();
    if(program == NULL || gra.getErrorCount() > 0){
        mylog::error << "Grammar analyzer failed.";
        exit(-1);
    }
    file.close();
    mylog::info << "Grammar analyzing Done.";

    // semantics analyze
    mylog::info << "Doing semantics analyzing...";
    NameTable tab;
    vector<FuncTuple*> func_tuples = program->dumpFunc(tab);

    if(Element::error_count > 0){
        mylog::error << "Semantics analyzer failed.";
        exit(-1);
    }
    mylog::info << "Semantics analyzing Done.";

    // claim global reg allocator
    GlobalRegAllocator *global_reg_allocator = NULL;

    // Start modify
    // remove const vars
    ConstVarRemover::work(tab, func_tuples);

    outputTuples(tab, func_tuples);
    mylog::tup << "\n";

    if(MODIFY){
        mylog::info << "Modify is on.";
        // split basic blocks
        vector<FuncBlock*> func_blocks = BasicBlockSplitter::work(func_tuples);

        mylog::debug << "Start dump basic blocks." << "\n";
        mylog::debug << "---------------------------" << "\n";
        for(FuncBlock* func_block : func_blocks){
            mylog::debug << func_block->toString() << "\n";
        }
        mylog::debug << "---------------------------" << "\n";
        mylog::debug << "Dump done." << "\n";

        // DAG modify
        if(DAG_MODIFY){
            mylog::info << "DAG Modify is on.";
            for(FuncBlock *func_block: func_blocks){
                for(BasicBlock *block: func_block->blocks){
                    dag::Worker::work(tab.getFuncNameTable(func_block->func_entry->name), block);
                }
            }
            // TODO free origin func_tuples after DAG modify
        }

        // Data Flow analyzing
        if(FLOW_ANALYZE){
            mylog::info << "Data Flow analyzing is on."
                        << "Its subject modifying will be done too.";
            vector<FlowFuncBlock*> flow_func_blocks;
            for(FuncBlock *func_block: func_blocks)
                flow_func_blocks.push_back(new FlowFuncBlock(func_block));

            mylog::debug << "Start dump data flow analyzing's result.";
            for(FlowFuncBlock *flow_func_block: flow_func_blocks)
                mylog::debug << flow_func_block->toString();
            mylog::debug << "Dump done.";

            // dump FlowFuncBlock to FuncBlock
            mylog::info << "Data Flow's subject modifies are all done.";
            func_blocks.clear();
            for(FlowFuncBlock *flow_func_block: flow_func_blocks)
                func_blocks.push_back(flow_func_block->toFuncBlock());

            // TODO build graph-global_reg_allocator
        }

        // dump func_tuples from func_blocks after all modify
        mylog::info << "Modify is finished.";
        func_tuples.clear();
        for(FuncBlock *func_block: func_blocks)
            func_tuples.push_back(func_block->dumpFuncTuple());

        mylog::tup << "After modify---" << "\n";
        outputTuples(tab, func_tuples);
    }

    // check global_reg_allocator, if still NULL, build linear one.
    if(!global_reg_allocator)
        global_reg_allocator = new LinearGlobalRegAllocator(tab);
    mylog::info << "Using " + global_reg_allocator->name();

    // MIPS backend
    mylog::info << "Doing MIPS backend transforming...";
    Backend backend;
    vector<DataCmd*> data_cmds;
    vector<InstCmd*> inst_cmds;
    backend.trans(tab, global_reg_allocator, func_tuples, &data_cmds, &inst_cmds);

    mylog::ass << ".data" << "\n";
    for(DataCmd *cmd: data_cmds){
        mylog::ass << cmd->toString() << "\n";
    }

    mylog::ass << "\n";

    mylog::ass << ".text" << "\n";
    for(InstCmd *cmd: inst_cmds){
        mylog::ass << cmd->toString() << "\n";
    }
    mylog::info << "MIPS backend transforming done.";
    

    // Release memory
    delete program;
    for(auto func_tuple: func_tuples)
        delete func_tuple;
    mylog::info << "All work finished.";
}
