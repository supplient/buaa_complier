#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include <string>

#include "symbol.h"
#include "Tuple.h"
#include "NameTable.h"
#include "log.h"
#include "NameUtil.h"
#include "TempVarPool.h"

using namespace std;

class Element{
    public:
        Element(){}
        virtual ~Element(){}
        virtual vector<Tuple*> dump(NameTable &tab){
            throw string("Element: dump not implemented.");
            vector<Tuple*> res; 
            return res;
        }

        void errorRepo(string msg){
            errorRepo(msg, start_line, end_line);
        }

        void errorRepo(string msg, int start_line, int end_line){
            // TODO print origin code line.
            cerr << "Semantics Error from line[" << start_line << "] to line[" << end_line << "]--" << endl;
            cerr << "\t" << msg << endl;
        }

        int start_line;
        int end_line;
};

#endif//ELEMENT_H