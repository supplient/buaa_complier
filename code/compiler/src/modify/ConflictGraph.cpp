#include "ConflictGraph.h"

vector<const VarEntry*> extractVarMapKeys(const VarMap &map){
    vector<const VarEntry*> res;
    for(auto pair: map)
        res.push_back(pair.first);
    return res;
}
