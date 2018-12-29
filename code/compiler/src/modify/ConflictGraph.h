#ifndef CONFLICT_GRAPH_H
#define CONFLICT_GRAPH_H

#include "FlowFuncBlock.h"
#include <set>

vector<const VarEntry*> extractVarMapKeys(const VarMap &map);

class ConflictGraph{
public:
    ConflictGraph(FlowFuncBlock *func_block){

        for(auto block: func_block->blocks){
            for(auto tuple: block->tuples){
                vector<const VarEntry*> actives = extractVarMapKeys(tuple->active_map);
                // We also do not allow def and use share one global reg.
                vector<const VarEntry*> defs = tuple->def();
                actives.insert(actives.end(), defs.begin(), defs.end());
                checkOnePoint(actives);
            }//for(auto tuple: block->tuples){

            vector<const VarEntry*> actives = extractVarMapKeys(block->active_map);
            checkOnePoint(actives);
        }//for(auto block: func_block->blocks){

    }

    void checkOnePoint(vector<const VarEntry*> vars){
        // remove param
        for(auto it=vars.begin(); it!=vars.end(); ){
            if((*it)->is_param){
                vars.erase(it);
                if(it == vars.end())
                    break;
            }
            else
                it++;
        }

        // add conflict edge
        for(int i=0; i<vars.size(); i++)
            for(int j=i+1; j<vars.size(); j++)
                addEdge(vars[i], vars[j]);
                
        // check no conflict var, add single node for them
        if(vars.size() == 1)
            addVar(vars[0]);
    }

    bool addVar(const VarEntry *var){
        if(mat.find(var) != mat.end())
            return false;

        mat[var].insert(NULL);
        mat[var].erase(NULL);
        return true;
    }

    bool removeVar(const VarEntry *var){
        if(mat.find(var) == mat.end())
            return false;

        for(auto pair: mat)
            pair.second.erase(var);
        mat.erase(var);
        return true;
    }

    unsigned int varNum(){
        return mat.size();
    }

    const VarEntry* searchVarConflictLess(unsigned int n){
        for(auto pair: mat){
            if(pair.second.size() < n)
                return pair.first;
        }
        return NULL;
    }

    const VarEntry* giveVarToGiveUp(){
        if(mat.size() < 1)
            return NULL;

        // give up the var conflict with most vars
        const VarEntry *res = NULL;
        unsigned int max_n = 0;
        for(auto pair: mat){
            if(pair.second.size() >= max_n){
                max_n = pair.second.size();
                res = pair.first;
            }
        }

        return res;
    }

    set<const VarEntry*> conflictWith(const VarEntry *var) const{
        auto it = mat.find(var);
        if(it == mat.end())
            throw string("ConflictGraph: Invalid var in the graph: " + var->name);
        return it->second;
    }

    string toString(){
        string s = "";
        for(auto pair: mat){
            s += NameUtil::genEntryName(pair.first) + ":";
            for(auto var: pair.second)
                s += " " + NameUtil::genEntryName(var);
            s += "\n";
        }
        return s;
    }

private:
    map<const VarEntry*, set<const VarEntry*> > mat;

    void addEdge(const VarEntry *u, const VarEntry *v){
        mat[u].insert(v);
        mat[v].insert(u);
    }
};

#endif//CONFLICT_GRAPH_H