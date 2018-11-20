#ifndef NAME_TABLE_H
#define NAME_TABLE_H

#include <vector>
#include <string>
#include <map>

using namespace std;

class FuncNameTable;

class NameTableEntry
{
public:
    NameTableEntry(FuncNameTable *owner):owner(owner){}

    string toString(){
        // TODO
        return "";
    }

private:
    FuncNameTable *owner;
};

class FuncNameTable
{
public:
    FuncNameTable(const string &name):func_name(name){}

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
    string toString(){
        string s = "";
        for(auto it: func_map){
            s += it.second->toString();
        }
        return s;
    }

private:
    map<string, FuncNameTable*> func_map;
};

#endif//NAME_TABLE_H
