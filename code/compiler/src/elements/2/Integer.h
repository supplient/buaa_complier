#ifndef INTEGER_H
#define INTEGER_H

#include <vector>

#include "Element.h"

using namespace std;

class Integer : public Element
{
    public:
        Integer();
        virtual ~Integer();

        int value;
};

#endif // CONST_DECL_H
