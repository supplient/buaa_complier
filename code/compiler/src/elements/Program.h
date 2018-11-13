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
    Program(/* args */);
    ~Program();

    vector<ConstDecl*> const_decl_list;
    vector<VarDecl*> var_decl_list;
    vector<FuncDefine*> func_define_list;
    MainFunc* main_func;
};


#endif//PROGRAM_H
