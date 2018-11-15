#ifndef CONST_DECL_H
#define CONST_DECL_H

#include <vector>

#include "Element.h"
#include "ConstDefine.h"

using namespace std;

class ConstDecl : public Element
{
    public:
        ConstDecl();
        virtual ~ConstDecl();

        vector<ConstDefine*> const_define_list;
};

#endif // CONST_DECL_H
