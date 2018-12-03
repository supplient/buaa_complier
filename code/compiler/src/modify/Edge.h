#ifndef EDGE_H
#define EDGE_H

class BasicBlock;

class Edge
{
public:
    enum TYPE{
        NORMAL,
        JUMP,
        BRANCH,
    };

    Edge(BasicBlock *from, BasicBlock *to, TYPE type)
        :from(from), to(to), type(type)
    {}

    BasicBlock *from;
    BasicBlock *to;
    TYPE type;
};

#endif//EDGE_H