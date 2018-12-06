#include "Node.h"

namespace dag{
    string Node::toString(){
        string s = "[Node " + to_string(index) + "]\n";
        s += "\tFathers:";
        for(OpNode *father: fathers)
            s += " " + to_string(father->index);
        s += "\n";
        return s;
    }
}