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
        if(const_decl){
            Tuples sub_tuples = const_decl->dump(tab, sem::GLOBAL_FUNC_NAME);
            tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());
        }

        if(var_decl){
            Tuples sub_tuples = var_decl->dump(tab, sem::GLOBAL_FUNC_NAME);
            tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());
        }

        // TODO func_define_list

        // TODO main_func

        return tuples;
    }
};


#endif//PROGRAM_H
