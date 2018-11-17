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
        ConstDefine();
        virtual ~ConstDefine();

        vector<string> ident_list;
};

class IntConstDefine : public ConstDefine
{
    public:
        IntConstDefine(){}
        virtual ~IntConstDefine(){}

        vector<Integer*> integer_list;
};

class CharConstDefine : public ConstDefine
{
    public:
        CharConstDefine(){}
        virtual ~CharConstDefine(){}

        vector<char> char_list;
};

#endif // CONST_DEFINE_H
