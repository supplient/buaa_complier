#ifndef CONST_DEFINE_H
#define CONST_DEFINE_H

#include <string>
#include <vector>

#include "Element.h"
#include "Integer.h"

using namespace std;

class ConstDefine : public Element
{
    public:
        ConstDefine(){}
        virtual ~ConstDefine(){}
        virtual Tuples dump(NameTable &tab, string filename)=0;

        vector<string> ident_list;
};

class IntConstDefine : public ConstDefine
{
    public:
        IntConstDefine(){}
        virtual ~IntConstDefine(){}

        vector<Integer*> integer_list;

        virtual Tuples dump(NameTable &tab, string func_name){
            Tuples tuples;

            if(ident_list.size() != integer_list.size()){
                log::fatal << "const define: ident size != integer size.";
                exit(-1);
            }

            // TODO dump info tuples

            // fill table
            for(unsigned int i=0;i<ident_list.size();i++){
                if(!tab.insertIntConst(func_name, ident_list[i], integer_list[i]->value)){
                    errorRepo("multi defination for " + ident_list[i]);
                }
            }

            return tuples;
        }
};

class CharConstDefine : public ConstDefine
{
    public:
        CharConstDefine(){}
        virtual ~CharConstDefine(){}

        vector<char> char_list;

        virtual Tuples dump(NameTable &tab, string func_name){
            Tuples tuples;

            if(ident_list.size() != char_list.size()){
                log::fatal << "const define: ident size != char size.";
                exit(-1);
            }

            // TODO dump info tuples

            // fill table
            for(unsigned int i=0;i<ident_list.size();i++){
                if(!tab.insertCharConst(func_name, ident_list[i], char_list[i])){
                    errorRepo("multi defination for " + ident_list[i]);
                }
            }

            return tuples;
        }
};

#endif // CONST_DEFINE_H
