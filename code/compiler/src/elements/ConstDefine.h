#ifndef CONST_DEFINE_H
#define CONST_DEFINE_H

#include "Element.h"


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
};

class CharConstDefine : public ConstDefine
{
    public:
        CharConstDefine(){}
        virtual ~CharConstDefine(){}
};

#endif // CONST_DEFINE_H
