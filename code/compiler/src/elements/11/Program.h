#ifndef PROGRAM_H
#define PROGRAM_H

#include "Element.h"
#include "ConstDecl.h"
#include "VarDecl.h"
#include "FuncDefine.h"
#include "MainFunc.h"
#include "FuncTuple.h"

using namespace std;

class Program:public Element
{
public:
    Program(){}
    virtual ~Program(){}

    ConstDecl* const_decl;
    VarDecl* var_decl;
    vector<FuncDefine*> func_define_list;
    MainFunc* main_func;

    vector<FuncTuple*> dumpFunc(NameTable &tab){
        vector<FuncTuple*> func_tuples;

        // dump const decl if exist
        if(const_decl){
            Tuples sub_tuples = const_decl->dump(tab, sem::GLOBAL_FUNC_NAME);
            //tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());
        }

        // dump var decl if exist
        if(var_decl){
            Tuples sub_tuples = var_decl->dump(tab, sem::GLOBAL_FUNC_NAME);
            //tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());
        }

        // dump func define if exist
        for(FuncDefine *func_define: func_define_list){
            if(!func_define)
                throw string("Func Define should never be NULL.");
            Tuples sub_tuples = func_define->dump(tab);
            FuncTuple *func_tuple = new FuncTuple(sub_tuples, func_define->func_name);
            func_tuples.push_back(func_tuple);
        }

        // dump main_func
        Tuples main_tuples = main_func->dump(tab);
        FuncTuple *func_tuple = new FuncTuple(main_tuples, sem::MAIN_FUNC_NAME);
        func_tuples.push_back(func_tuple);

        return func_tuples;
    }
};


#endif//PROGRAM_H
