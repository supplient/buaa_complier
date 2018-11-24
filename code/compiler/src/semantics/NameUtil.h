#ifndef NAME_UTIL_H
#define NAME_UTIL_H

#include <string>

using namespace std;

class NameTableEntry;

class NameUtil
{
public:
    static string genFuncLabel(const string &func_name){
        return "$" + func_name;
    }

    static string genUniBranchLabel(){
        static int branch_label_count = 0;
        string res = "$$" + to_string(branch_label_count);
        branch_label_count++;
        return res;
    }

    static string genEntryName(const NameTableEntry *entry);

    static string genTempVarName(int index){
        return "#" + to_string(index);
    }

    static string getIntReturnVarName(){
        return "#RV#INT";
    }

    static string getCharReturnVarName(){
        return "#RV#CHAR";
    }
};

#endif//NAME_UTIL_H