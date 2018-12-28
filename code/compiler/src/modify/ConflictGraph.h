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

                // remove param
                for(auto it=actives.begin(); it!=actives.end(); ){
                    if((*it)->is_param){
                        actives.erase(it);
                        if(it == actives.end())
                            break;
                    }
                    else
                        it++;
                }

                // add conflict edge
                for(int i=0; i<actives.size(); i++)
                    for(int j=i+1; j<actives.size(); j++)
                        addEdge(actives[i], actives[j]);
                
                // check no conflict var, add single node for them
                if(actives.size() == 1)
                    addVar(actives[0]);

            }//for(auto tuple: block->tuples){

            vector<const VarEntry*> actives = extractVarMapKeys(block->active_map);
            // remove param
            for(auto it=actives.begin(); it!=actives.end(); ){
                if((*it)->is_param){
                    actives.erase(it);
                    if(it == actives.end())
                        break;
                }
                else
                    it++;
            }
            // add conflict edge
            for(int i=0; i<actives.size(); i++)
                for(int j=i+1; j<actives.size(); j++)
                    addEdge(actives[i], actives[j]);
            // check no conflict var, add single node for them
            if(actives.size() == 1)// for no conflict vars
                addVar(actives[0]);

        }//for(auto block: func_block->blocks){

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