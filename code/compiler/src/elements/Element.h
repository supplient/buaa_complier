#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include <string>

using namespace std;

class Element{
    public:
        Element():ele_name("Element"){}
        //virtual bool evaluate()=0;

        string ele_name;
};

#endif//ELEMENT_H