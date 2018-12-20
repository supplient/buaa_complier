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
            case sem::NEG:
            case sem::BEZ:
                ords.push_back(tuple->left);
                break;
            case sem::ADD:
            case sem::SUB:
            case sem::MUL:
            case sem::DIV:
            case sem::WARRAY:
            case sem::RARRAY:
            case sem::LESS:
            case sem::LESSOREQUAL:
            case sem::MORE:
            case sem::MOREOREQUAL:
            case sem::NOTEQUAL:
            case sem::EQUAL:
            case sem::BEQ:
                ords.push_back(tuple->left);
                ords.push_back(tuple->right);
                break;
            case sem::PARAM:
                ords.push_back(tuple->res);
                break;
            case sem::OUTPUT:
                if(tuple->left)
                    ords.push_back(tuple->left);
                if(tuple->right)
                    ords.push_back(tuple->right);
                break;
            case sem::LABEL:
            case sem::FUNC:
            case sem::CALL:
            case sem::RET:
            case sem::INPUT:
            case sem::JMP:
                ;// Do Nothing.
                break;
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

        switch(tuple->op){
            case sem::ASSIGN:
            case sem::NEG:
            case sem::ADD:
            case sem::SUB:
            case sem::MUL:
            case sem::DIV:
            case sem::WARRAY:
            case sem::RARRAY:
            case sem::INPUT:
            case sem::LESS:
            case sem::LESSOREQUAL:
            case sem::MORE:
            case sem::MOREOREQUAL:
            case sem::NOTEQUAL:
            case sem::EQUAL:
                ord = tuple->res;
                break;
            case sem::LABEL:
            case sem::FUNC:
            case sem::PARAM:
            case sem::CALL:
            case sem::RET:
            case sem::OUTPUT:
            case sem::BEZ:
            case sem::BEQ:
            case sem::JMP:
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