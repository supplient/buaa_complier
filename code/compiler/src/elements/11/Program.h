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

        // create start label
        string start_label_name = NameUtil::genFuncLabel(sem::GLOBAL_FUNC_NAME);
        Tuple *start_label = new Tuple();
        start_label->op = sem::LABEL;
        start_label->res = new Operand(start_label_name);

        // jump to start label
        Tuple *jmp_tuple = new Tuple();
        jmp_tuple->op = sem::JMP;
        jmp_tuple->res = new Operand(start_label_name);
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

        // mark the program start
        tuples.push_back(start_label);

        // call main function
        Tuple *call_tuple = new Tuple();
        call_tuple->op = sem::CALL;
        call_tuple->res = new Operand(tab.lookUp(sem::GLOBAL_FUNC_NAME, sem::MAIN_FUNC_NAME));
        tuples.push_back(call_tuple);

        return tuples;
    }
};


#endif//PROGRAM_H
