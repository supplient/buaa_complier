#ifndef NAME_TABLE_H
#define NAME_TABLE_H

#include <vector>
#include <string>
#include <map>

#include "NameTableEntry.h"

using namespace std;

namespace sem{
    const string GLOBAL_FUNC_NAME = "@global";
}

class FuncNameTable;

class FuncNameTable
{
public:
    FuncNameTable(const string &name):func_name(name){}

    NameTableEntry* lookUp(string name){
        for(NameTableEntry *entry : entries){
            if(entry->name == name)
                return entry;
        }
        return NULL;
    }

    bool insertVar(string name, sym::SYMBOL type, int dim){
        if(lookUp(name))
            return false; // the same name has been declared in this func
        VarEntry *entry = new VarEntry(name, this, type, dim);
        entries.push_back(static_cast<NameTableEntry*>(entry));
        return true;
    }

    bool insertIntConst(string name, int value){
        if(lookUp(name))
            return false;
        ConstEntry *entry = new ConstEntry(name, this, value);
        entries.push_back(static_cast<NameTableEntry*>(entry));
        return true;
    }

    bool insertCharConst(string name, char value){
        if(lookUp(name))
            return false;
        ConstEntry *entry = new ConstEntry(name, this, value);
        entries.push_back(static_cast<NameTableEntry*>(entry));
        return true;
    }

    string toString(){
        string s = "";
        for(auto it: entries){
            s += it->toString() + "\n";
        }
        return s;
    }

private:
    string func_name;
    vector<NameTableEntry*> entries;
};

class NameTable
{
public:
    NameTable(){
        // Init global table
        func_map[sem::GLOBAL_FUNC_NAME] = new FuncNameTable(sem::GLOBAL_FUNC_NAME);
        global_tab = func_map[sem::GLOBAL_FUNC_NAME];
    }

    NameTableEntry* lookUpOnlyCurrentFunc(string func_name, string name){
        NameTableEntry *res = NULL;
        auto it = func_map.find(func_name);
        if(it != func_map.end()){
            // have created FuncNameTable with the name of `func_name`
            res = it->second->lookUp(name);
        }
        return res;
    }

    NameTableEntry* lookUp(string func_name, string name){
        NameTableEntry *res = NULL;
        res = lookUpOnlyCurrentFunc(func_name, name);

        if(res == NULL){
            // search in current func failed.
            // search global
            res = global_tab->lookUp(name);
        }

        return res;
    }

    // Maybe useless functions, do not add more if no need.
    VarEntry* lookUpVar(string func_name, string name){
        NameTableEntry* entry = lookUp(func_name, name);
        return dynamic_cast<VarEntry*>(entry);
    }
    // TODO lookUpConst, lookUpFunc

    bool insertVar(string func_name, string name, sym::SYMBOL type, int dim){
        return func_map[func_name]->insertVar(name, type, dim);
    }

    bool insertIntConst(string func_name, string name, int value){
        return func_map[func_name]->insertIntConst(name, value);
    }

    bool insertCharConst(string func_name, string name, char value){
        return func_map[func_name]->insertCharConst(name, value);
    }
    // TODO insertConst, insertFunc

    string toString(){
        string s = "";
        for(auto it: func_map){
            s += it.second->toString();
        }
        return s;
    }

private:
    FuncNameTable* global_tab;
    map<string, FuncNameTable*> func_map;
};

#endif//NAME_TABLE_H
