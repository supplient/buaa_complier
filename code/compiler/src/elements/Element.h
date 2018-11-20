#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include <string>

#include "symbol.h"

using namespace std;

class Element{
    public:
        Element(){}
        //virtual bool evaluate()=0;

        int start_line;
        int end_line;
};

#endif//ELEMENT_H