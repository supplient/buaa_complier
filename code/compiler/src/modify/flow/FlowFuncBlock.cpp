#include "FlowFuncBlock.h"

unsigned int FlowFuncBlock::tuple_index = 0;

void mergeVarMap(VarMap &res, const VarMap &from){
    for(auto pair: from){
        vector<FlowTuple*> &res_vec = res[pair.first];
        for(auto tuple: pair.second){
            auto it = find(res_vec.begin(), res_vec.end(), tuple);
            if(it != res_vec.end())
                continue; // exist
            res_vec.push_back(tuple);
        }
    }
}