#include "NameTable.h"
#include <iostream>

using namespace std;

FuncNameTable::FuncNameTable(const string &name)
    :func_name(name)
{
}

void FuncNameTable::removeConstVar(){
    auto it = entries.begin();
    while(it != entries.end()){
        NameTableEntry* entry = *it;
        VarEntry* var_entry = dynamic_cast<VarEntry*>(entry);
        if(!var_entry || entry->entry_type != sem::VAR_ENTRY){
            it++;
            continue;
        }
        if(!var_entry->is_const){
            it++;
            continue;
        }
        if(NameUtil::isSpecialVarName(var_entry->name)){
            it++;
            continue;
        }

        entries.erase(it);
    }
}

FuncEntry* NameTable::insertFunc(string name, sym::SYMBOL return_type, const vector<VarEntry*> param_list, Tuple *start_tuple){
    if(func_map.find(name) == func_map.end())
        func_map[name] = new FuncNameTable(name);
    return func_map[sem::GLOBAL_FUNC_NAME]->insertFunc(name, return_type, param_list, start_tuple);
}

void NameTable::removeConstVar(){
    for(auto pair: func_map){
        cerr << pair.first << endl;
        pair.second->removeConstVar();
    }

}
