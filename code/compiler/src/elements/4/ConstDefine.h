#ifndef CONST_DEFINE_H
#define CONST_DEFINE_H

#include <string>

#include "Element.h"
#include "Integer.h"

using namespace std;

class ConstDefine : public Element
{
    public:
        ConstDefine();
        virtual ~ConstDefine();
};

class IntConstDefine : public ConstDefine
{
    public:
        IntConstDefine(){}
        virtual ~IntConstDefine(){}

        string ident;
        Integer *integer;
};

class CharConstDefine : public ConstDefine
{
    public:
        CharConstDefine(){}
        virtual ~CharConstDefine(){}
};

#endif // CONST_DEFINE_H
