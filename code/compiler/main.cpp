#include <iostream>
#include <string>
#include "log.h"

using namespace std;

void modiTest(string filename);

void modiInterface();

bool STO = false;// standard tuple output

bool FUNC_PREFIX_ENTRY_NAME = false;
bool MIPS_TUPLE_OUTPUT = true;

bool MODIFY = true;
bool DAG_MODIFY = false & MODIFY;
bool FLOW_ANALYZE = true & MODIFY;

int main()
{
    try{
        //modiTest("modi_test.c0");
        modiTest("sample/complex/c0");

        //modiInterface();
    }
    catch(string s){
        mylog::fatal << s;
    }

    char stop;
    cin >> stop;
    return 0;
}
