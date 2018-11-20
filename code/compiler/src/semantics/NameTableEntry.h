#ifndef NAME_TABLE_ENTRY_H
#define NAME_TABLE_ENTRY_H

#include <string>

#include "symbol.h"

class FuncNameTable;

using namespace std;

namespace sem{
    const int MAX_ENTRY_TYPE_NUM = 20;
    const int MAX_ENTRY_TYPE_LEN = 30;
    enum ENTRY_TYPE{
        EMPTY_ENTRY,
        VAR_ENTRY,
        CONST_ENTRY,
        FUNC_ENTRY
    };

    extern char ENTRY_TYPE_NAME[MAX_ENTRY_TYPE_NUM][MAX_ENTRY_TYPE_LEN];
}

class NameTableEntry
{
public:
    NameTableEntry(const string &name, sem::ENTRY_TYPE entry_type, FuncNameTable *owner)
        :name(name), entry_type(entry_type), owner(owner){
    }

    virtual string toString(){
        string s = sem::ENTRY_TYPE_NAME[entry_type];
        s += " " + name;
        return s;
    }

    const sem::ENTRY_TYPE entry_type;
    const string name;

private:
    FuncNameTable *owner;
};

class VarEntry: public NameTableEntry
{
public:
    VarEntry(const string &name, FuncNameTable *owner, sym::SYMBOL type, int dim)
        :NameTableEntry(name, sem::VAR_ENTRY, owner), type(type), dim(dim){
    }

    virtual string toString(){
        string s = NameTableEntry::toString();
        s += " ";
        s += sym::SYMBOL_NAME[type];
        s += " " + to_string(dim);
        return s;
    }

    const sym::SYMBOL type;
    const int dim;
};

// TODO ConstEntry, FuncEntry

#endif//NAME_TABLE_ENTRY_H
