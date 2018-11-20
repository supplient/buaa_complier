#ifndef VAR_DECL_H
#define VAR_DECL_H

#include "Element.h"
#include "VarDefine.h"

class VarDecl : public Element
{
    public:
        VarDecl(){}
        virtual ~VarDecl(){}

        vector<VarDefine*> var_define_list;

        virtual Tuples dump(NameTable &tab, string func_name){
            Tuples tuples;

            for(VarDefine *var_define: var_define_list){
                Tuples def_tuples = var_define->dump(tab, func_name);
                tuples.insert(tuples.end(), def_tuples.begin(), def_tuples.end());
            }

            return tuples;
        }
};

#endif // VAR_DECL_H
