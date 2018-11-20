#ifndef CONST_DECL_H
#define CONST_DECL_H

#include <vector>

#include "Element.h"
#include "ConstDefine.h"

using namespace std;

class ConstDecl : public Element
{
    public:
        ConstDecl(){}
        virtual ~ConstDecl(){}

        vector<ConstDefine*> const_define_list;

        virtual Tuples dump(NameTable &tab, string func_name){
            Tuples tuples;

            for(ConstDefine *const_define: const_define_list){
                Tuples def_tuples = const_define->dump(tab, func_name); // TODO
                tuples.insert(tuples.end(), def_tuples.begin(), def_tuples.end());
            }

            return tuples;
        }
};

#endif // CONST_DECL_H
