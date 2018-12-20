#ifndef FLOW_TUPLE_H
#define FLOW_TUPLE_H

#include "Tuple.h"

#include <map>

class FlowTuple;
typedef map<const VarEntry*, vector<FlowTuple*> > VarMap;

class FlowTuple{
public:
    FlowTuple(Tuple *tuple);

    string toString(string tab=""){
        string res = tuple->toString();
        res = tab + "[" + to_string(index) + "]" + res;
        if(live_map.size() > 0){
            res += "\t[L";
            for(auto pair: live_map){
                res += " " + NameUtil::genEntryName(pair.first) + "(";
                for(unsigned int i=0; i<pair.second.size(); i++){
                    res += to_string(pair.second[i]->index);
                    if(i < pair.second.size()-1)
                        res += " ";
                }
                res += ")";
            }
            res += "]";
        }

        if(active_map.size() > 0){
            res += "\t[A";
            for(auto pair: active_map){
                res += " " + NameUtil::genEntryName(pair.first) + "(";
                for(unsigned int i=0; i<pair.second.size(); i++){
                    res += to_string(pair.second[i]->index);
                    if(i < pair.second.size()-1)
                        res += " ";
                }
                res += ")";
            }
            res += "]";
        }

        return res;
    }

    // Note: We do not count global vars
    vector<const VarEntry*> use();
    vector<const VarEntry*> def();

    void reset(){
        live_map.clear();
        active_map.clear();
    }

    unsigned int index;
    Tuple *tuple;

    // TODO fill these map
    VarMap live_map; // The alive vars(with its def tuple) at the head of the tuple
    VarMap active_map; // The active vars(with its use tuple) at the end of the tuple
};

#endif//FLOW_TUPLE_H