#ifndef NAME_TABLE_H
#define NAME_TABLE_H

#include <vector>
#include <string>
#include <map>

#include "NameTableEntry.h"
#include "NameUtil.h"

using namespace std;

namespace sem{
    const string GLOBAL_FUNC_NAME = "@global";
    const string MAIN_FUNC_NAME = "main";
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

    VarEntry* insertVar(string name, sym::SYMBOL type, int dim){
        if(lookUp(name))
            return NULL; // the same name has been declared in this func
        VarEntry *entry = new VarEntry(name, this, type, dim);
        entries.push_back(static_cast<NameTableEntry*>(entry));
        return entry;
    }

    VarEntry* insertIntConst(string name, int value){
        if(lookUp(name))
            return NULL;
        VarEntry *entry = new VarEntry(name, this, value);
        entries.push_back(static_cast<NameTableEntry*>(entry));
        return entry;
    }

    VarEntry* insertCharConst(string name, char value){
        if(lookUp(name))
            return NULL;
        VarEntry *entry = new VarEntry(name, this, value);
        entries.push_back(static_cast<NameTableEntry*>(entry));
        return entry;
    }

    FuncEntry* insertFunc(string name, sym::SYMBOL return_type, const vector<VarEntry*> param_list, Tuple *start_tuple){
        if(lookUp(name))
            return NULL;
        FuncEntry *entry = new FuncEntry(name, this, return_type, param_list, start_tuple);
        entries.push_back(static_cast<NameTableEntry*>(entry));
        return entry;
    }

    vector<VarEntry*> getUnconstVars(){
        vector<VarEntry*> res;
        
        for(NameTableEntry* entry: entries){
            VarEntry* var_entry = dynamic_cast<VarEntry*>(entry);
            if(!var_entry || entry->entry_type != sem::VAR_ENTRY)
                continue;
            if(var_entry->is_const)
                continue;
            if(NameUtil::isSpecialVarName(var_entry->name))
                continue;
            res.push_back(var_entry);
        }

        return res;
    }

    string toString(){
        string s = "";
        for(auto it: entries){
            s += it->toString() + "\n";
        }
        return s;
    }

    const string func_name;

private:
    vector<NameTableEntry*> entries;
};

class NameTable
{
public:
    NameTable(){
        // Init global table
        func_map[sem::GLOBAL_FUNC_NAME] = new FuncNameTable(sem::GLOBAL_FUNC_NAME);
        global_tab = func_map[sem::GLOBAL_FUNC_NAME];

        // Insert return vars
        global_tab->insertVar(NameUtil::getIntReturnVarName(), sym::INT, 0);
        global_tab->insertVar(NameUtil::getCharReturnVarName(), sym::CHAR, 0);
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
    VarEntry* lookUpVarOnlyCurrentFunc(string func_name, string name){
        NameTableEntry* entry = lookUpOnlyCurrentFunc(func_name, name);
        if(entry)
            return dynamic_cast<VarEntry*>(entry);
        else
            return NULL;
    }

    VarEntry* lookUpVar(string func_name, string name){
        NameTableEntry* entry = lookUp(func_name, name);
        if(entry)
            return dynamic_cast<VarEntry*>(entry);
        else
            return NULL;
    }

    FuncEntry* lookUpFunc(string name){
        NameTableEntry* entry = global_tab->lookUp(name);
        if(entry)
            return dynamic_cast<FuncEntry*>(entry);
        else
            return NULL;
    }

    VarEntry* insertVar(string func_name, string name, sym::SYMBOL type, int dim){
        if(func_map.find(func_name) == func_map.end())
            func_map[func_name] = new FuncNameTable(func_name);
        return func_map[func_name]->insertVar(name, type, dim);
    }

    VarEntry* insertIntConst(string func_name, string name, int value){
        if(func_map.find(func_name) == func_map.end())
            func_map[func_name] = new FuncNameTable(func_name);
        return func_map[func_name]->insertIntConst(name, value);
    }

    VarEntry* insertCharConst(string func_name, string name, char value){
        if(func_map.find(func_name) == func_map.end())
            func_map[func_name] = new FuncNameTable(func_name);
        return func_map[func_name]->insertCharConst(name, value);
    }

    FuncEntry* insertFunc(string name, sym::SYMBOL return_type, const vector<VarEntry*> param_list, Tuple *start_tuple){
        return func_map[sem::GLOBAL_FUNC_NAME]->insertFunc(name, return_type, param_list, start_tuple);
    }

    FuncNameTable* getFuncNameTable(string func_name){
        if(func_map.find(func_name) == func_map.end())
            return NULL;
        return func_map[func_name];
    }

    vector<FuncNameTable*> getAllLocalFuncNameTable(){
        vector<FuncNameTable*> res;
        for(auto pair: func_map){
            if(pair.first == sem::GLOBAL_FUNC_NAME)
                continue;
            res.push_back(pair.second);
        }
        return res;
    }

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
