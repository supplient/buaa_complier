#ifndef PROGRAM_H
#define PROGRAM_H

#include "Element.h"
#include "ConstDecl.h"
#include "VarDecl.h"
#include "FuncDefine.h"
#include "MainFunc.h"

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

    virtual vector<Tuple*> dump(NameTable &tab){
        vector<Tuple*> tuples;

        // TODO const_decl

        if(var_decl){
            Tuples var_decl_tuples = var_decl->dump(tab, sem::GLOBAL_FUNC_NAME);
            tuples.insert(tuples.end(), var_decl_tuples.begin(), var_decl_tuples.end());
        }

        // TODO func_define_list

        // TODO main_func

        return tuples;
    }
};


#endif//PROGRAM_H
