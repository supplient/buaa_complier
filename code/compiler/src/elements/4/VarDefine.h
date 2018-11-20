#ifndef VAR_DEFINE_H
#define VAR_DEFINE_H

#include "Element.h"
#include "symbol.h"

class VarDefine: public Element
{
    public:
        virtual ~VarDefine(){}
        sym::SYMBOL type;
        vector<string> ident_list;
        vector<unsigned int> dim_list;

        Tuples dump(NameTable &tab, string func_name){
            Tuples tuples;

            if(ident_list.size() != dim_list.size()){
                log::fatal << "var's number is not equal with dim's number";
                exit(-1);
            }

            // TODO dump info tuples

            // fill tab
            for(int i=0;i<ident_list.size();i++){
                string ident = ident_list[i];
                unsigned int dim = dim_list[i];

                if(!tab.insertVar(func_name, ident, type, dim)){
                    errorRepo("multi defination for " + ident);
                }
            }

            return tuples;
        }
};

#endif//VAR_DEFINE_H
