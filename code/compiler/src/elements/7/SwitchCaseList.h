#ifndef SWITCH_CASE_LIST_H
#define SWITCH_CASE_LIST_H

#include "Element.h"
#include "SwitchCase.h"

class SwitchCaseList: public Element
{
public:
    vector<SwitchCase*> case_list;
};

#endif// SWITCH_CASE_LIST_H