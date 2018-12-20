#ifndef FLOW_TUPLE_H
#define FLOW_TUPLE_H

#include "Tuple.h"

#include <map>

class FlowTuple;
typedef map<const VarEntry*, vector<FlowTuple*> > VarMap;

class FlowTuple{
public:
    FlowTuple(Tuple *tuple)
        :tuple(tuple)
    {
        static unsigned int index = 0;
        this->index = index++;
    }

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
    vector<const VarEntry*> use(){
        vector<const VarEntry*> res;
        vector<Operand*> ords;

        switch(tuple->op){
            case sem::ASSIGN:
            case sem::BEZ:
                ords.push_back(tuple->left);
                break;
            case sem::LESS:
            case sem::ADD:
                ords.push_back(tuple->left);
                ords.push_back(tuple->right);
                break;
            case sem::OUTPUT:
                if(tuple->left)
                    ords.push_back(tuple->left);
                if(tuple->right)
                    ords.push_back(tuple->right);
                break;
            case sem::FUNC:
            case sem::LABEL:
            case sem::JMP:
            case sem::RET:
            case sem::CALL:
                ;// Do Nothing.
                break;
            // TODO
            default:
                throw string("FlowTuple.use: Not implemented " + to_string(tuple->op));
        }

        for(Operand *ord: ords){
            if(ord->type == Operand::ENTRY){
                // Note: We do not count global vars
                if(VarEntry::isGlobalVar(ord->entry))
                    continue;
                res.push_back(ord->entry);
            }
        }

        return res;
    }

    vector<const VarEntry*> def(){
        vector<const VarEntry*> res;
        Operand *ord;
        // FUNC, ASSIGN, LABEL, LESS, BEZ, ADD, JMP, RET, CALL, OUTPUT

        switch(tuple->op){
            case sem::ASSIGN:
            case sem::LESS:
            case sem::ADD:
                ord = tuple->res;
                break;
            // TODO
            case sem::OUTPUT:
            case sem::BEZ:
            case sem::FUNC:
            case sem::LABEL:
            case sem::JMP:
            case sem::RET:
            case sem::CALL:
                ;// Do nothing.
                break;
            default:
                throw string("FlowTuple.def: Not implemented " + to_string(tuple->op));
        }
        
        if(ord->type == Operand::ENTRY){
            // Note: We do not count global vars
            if(!VarEntry::isGlobalVar(ord->entry))
                res.push_back(ord->entry);
        }

        return res;
    }

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