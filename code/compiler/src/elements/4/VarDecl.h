#ifndef VAR_DECL_H
#define VAR_DECL_H

#include "Element.h"
#include "VarDefine.h"

class VarDecl : public Element
{
    public:
        VarDecl();
        virtual ~VarDecl();

        vector<VarDefine*> var_define_list;
};

#endif // VAR_DECL_H
