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

        virtual Tuples dump(NameTable &tab){
            Tuples tuples;
            log::error << "Integer: should never dump Integer!";
            exit(-1);
            return tuples;
        }
};

#endif // CONST_DECL_H
