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
        string res = "$bra_" + to_string(branch_label_count);
        branch_label_count++;
        return res;
    }

    static string genUniStringLabel(){
        static int str_label_count = 0;
        string res = "$str_" + to_string(str_label_count);
        str_label_count++;
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

    static bool isSpecialVarName(string var_name){
        if(var_name.size() < 1)
            throw string("NameUtil: empty string should never be a var's name.");
        return var_name == getIntReturnVarName()
            || var_name == getCharReturnVarName();
    }

    static string genGlobalVarLabel(string var_name){
        return var_name;
    }
};

#endif//NAME_UTIL_H