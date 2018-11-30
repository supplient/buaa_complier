#ifndef NAME_TABLE_ENTRY_H
#define NAME_TABLE_ENTRY_H

#include <string>
#include <vector>

#include "symbol.h"
#include "log.h"

class Tuple;
class FuncNameTable;

using namespace std;

namespace sem{
    const int MAX_ENTRY_TYPE_NUM = 20;
    const int MAX_ENTRY_TYPE_LEN = 30;
    enum ENTRY_TYPE{
        EMPTY_ENTRY,
        VAR_ENTRY,
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

    string getOwnerName()const;

    virtual string toString(){
        string s = sem::ENTRY_TYPE_NAME[entry_type];
        s += " " + name;
        s += " [" + getOwnerName() + "]";
        return s;
    }

    const string name;
    const sem::ENTRY_TYPE entry_type;

private:
    FuncNameTable *owner;
};

class VarEntry: public NameTableEntry
{
public:
    VarEntry(const string &name, FuncNameTable *owner, sym::SYMBOL type, int dim, bool is_param=false, int param_index = 0, bool is_const=false)
        :NameTableEntry(name, sem::VAR_ENTRY, owner), is_param(is_param), param_index(param_index), type(type), dim(dim), is_const(is_const), int_value(0), char_value(0){
    }
    VarEntry(const string &name, FuncNameTable *owner, int value, bool is_param=false, int param_index = 0, bool is_const=true)
        :NameTableEntry(name, sem::VAR_ENTRY, owner), is_param(is_param), param_index(param_index), type(sym::INT), dim(0), is_const(is_const), int_value(value), char_value(0){
    }
    VarEntry(const string &name, FuncNameTable *owner, char value, bool is_param=false, int param_index = 0, bool is_const=true)
        :NameTableEntry(name, sem::VAR_ENTRY, owner), is_param(is_param), param_index(param_index), type(sym::CHAR), dim(0), is_const(is_const), int_value(0), char_value(value){
    }

    virtual string toString();

    const bool is_param;
    const unsigned int param_index;

    const sym::SYMBOL type;
    const int dim;

    const bool is_const;
    const int int_value;
    const char char_value;
};

class FuncEntry: public NameTableEntry
{
public:
    FuncEntry(const string &name, FuncNameTable *owner,
            sym::SYMBOL return_type, const vector<VarEntry*> param_list, Tuple *start_tuple)
            :   NameTableEntry(name, sem::FUNC_ENTRY, owner),
                return_type(return_type),
                param_list(param_list),
                start_tuple(start_tuple)
    {}

    virtual string toString(){
        string s = NameTableEntry::toString();
        s += " ";
        s += sym::SYMBOL_NAME[return_type];
        return s;
    }

    const sym::SYMBOL return_type;
    const vector<VarEntry*> param_list;
    const Tuple *start_tuple;
};

#endif//NAME_TABLE_ENTRY_H
