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

void outputTuples(mylog::FileNormal &out, NameTable &tab, vector<FuncTuple*> func_tuples){
    if(!STO){
        out << "Start dump name table." << "\n";
        out << "---------------------------" << "\n";
        out << tab.toString();
        out << "---------------------------" << "\n";
        out << "Dump done." << "\n";

        out << "\n"; 
    }

    out << "Start dump tuples." << "\n";
    out << "---------------------------" << "\n";
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
            out << s;
        }
    }
    for(FuncTuple* func_tuple : func_tuples){
        if(STO)
            out << func_tuple->toString(tab) << "\n";
        else
            out << func_tuple->toString() << "\n";
    }
    out << "---------------------------" << "\n";
    out << "Dump done." << "\n";
}


void outputMIPS(mylog::FileNormal &out, NameTable &tab, GlobalRegAllocator *global_reg_allocator, vector<FuncTuple*> func_tuples){
    Backend backend;
    vector<DataCmd*> data_cmds;
    vector<InstCmd*> inst_cmds;
    backend.trans(tab, global_reg_allocator, func_tuples, &data_cmds, &inst_cmds);

    out << ".data" << "\n";
    for(DataCmd *cmd: data_cmds){
        out << cmd->toString() << "\n";
    }

    out << "\n";

    out << ".text" << "\n";
    for(InstCmd *cmd: inst_cmds){
        out << cmd->toString() << "\n";
    }
}


void modiTest(string filename){
    mylog::FileNormal mid_prev("../../output/16061103_zzy_mid_prev.TXT");
    mylog::FileNormal mid_after("../../output/16061103_zzy_mid_after.TXT");
    mylog::FileNormal mips_prev("../../output/16061103_zzy_mips_prev.TXT");
    mylog::FileNormal mips_after("../../output/16061103_zzy_mips_after.TXT");

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

    // remove const vars
    ConstVarRemover::work(tab, func_tuples);

    outputTuples(mylog::tup, tab, func_tuples);
    outputTuples(mid_prev, tab, func_tuples);
    mylog::tup << "\n";

    // Start modify
    if(MODIFY){
        // MIPS Backend before modify
        global_reg_allocator = new LinearGlobalRegAllocator(tab);
        mylog::info << "Using " + global_reg_allocator->name();
        mylog::info << "Doing MIPS backend transforming before modify...";
        outputMIPS(mips_prev, tab, global_reg_allocator, func_tuples);
        mylog::info << "MIPS backend transforming done.";
        delete global_reg_allocator;
        global_reg_allocator = NULL;

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

            // build graph-global_reg_allocator
            if(global_reg_allocator)
                delete global_reg_allocator;
            global_reg_allocator = new GraphGlobalRegAllocator(flow_func_blocks);
        }

        // dump func_tuples from func_blocks after all modify
        mylog::info << "Modify is finished.";
        func_tuples.clear();
        for(FuncBlock *func_block: func_blocks)
            func_tuples.push_back(func_block->dumpFuncTuple());

        mylog::tup << "After modify---" << "\n";
        outputTuples(mylog::tup, tab, func_tuples);
        outputTuples(mid_after, tab, func_tuples);
    }

    // check global_reg_allocator, if still NULL, build linear one.
    if(!global_reg_allocator)
        global_reg_allocator = new LinearGlobalRegAllocator(tab);
    mylog::info << "Using " + global_reg_allocator->name();

    // MIPS backend
    mylog::info << "Doing MIPS backend transforming...";
    outputMIPS(mylog::ass, tab, global_reg_allocator, func_tuples);
    outputMIPS(mips_after, tab, global_reg_allocator, func_tuples);
    mylog::info << "MIPS backend transforming done.";


    // Release memory
    delete program;
    for(auto func_tuple: func_tuples)
        delete func_tuple;
    mylog::info << "All work finished.";
}
