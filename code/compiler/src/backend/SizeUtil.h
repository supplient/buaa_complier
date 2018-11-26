#ifndef SIZE_UTIL_H
#define SIZE_UTIL_H

namespace back
{
    typedef unsigned int size; // count by byte
} // back

class SizeUtil
{
public:
    static back::size regSize(){
        return 4;
    }

    static back::size charSize(){
        return 1;
    }

    static back::size intSize(){
        return 4;
    }
};

#endif//SIZE_UTIL_H