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

        // jump to main function
        Tuple *jmp_tuple = new Tuple();
        jmp_tuple->op = sem::JMP;
        jmp_tuple->res = new Operand(NameUtil::genFuncLabel(sem::MAIN_FUNC_NAME));
        tuples.push_back(jmp_tuple);

        // dump const decl if exist
        if(const_decl){
            Tuples sub_tuples = const_decl->dump(tab, sem::GLOBAL_FUNC_NAME);
            tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());
        }

        // dump var decl if exist
        if(var_decl){
            Tuples sub_tuples = var_decl->dump(tab, sem::GLOBAL_FUNC_NAME);
            tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());
        }

        // dump func define if exist
        for(FuncDefine *func_define: func_define_list){
            if(!func_define)
                throw string("Func Define should never be NULL.");
            Tuples sub_tuples = func_define->dump(tab);
            tuples.insert(tuples.end(), sub_tuples.begin(), sub_tuples.end());
        }

        // dump main_func
        Tuples main_tuples = main_func->dump(tab);
        tuples.insert(tuples.end(), main_tuples.begin(), main_tuples.end());

        return tuples;
    }
};


#endif//PROGRAM_H
