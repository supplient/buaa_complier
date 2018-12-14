#ifndef NAME_UTIL_H
#define NAME_UTIL_H

#include <string>

using namespace std;

class NameTableEntry;

class NameUtil
{
public:
    static string genFuncLabel(const string &func_name){
        return "$func_" + func_name;
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

    static string genUniqueDAGVarName(const VarEntry *entry){
        static map<const VarEntry*, int> cnt_tab;

        int index = 0;
        auto it = find(cnt_tab.begin(), cnt_tab.end(), entry);
        if(it == cnt_tab.end())
            cnt_tab[entry] = 1;
        else{
            index = it->second;
            it->second = it->second + 1;
        }

        return "@" + to_string(index) + "_" + genEntryName(entry);
    }

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
        return "$global_" + var_name;
    }

    static string intString;
    static string charString;
};

#endif//NAME_UTIL_H