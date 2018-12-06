#include <iostream>
#include <string>
#include "log.h"

using namespace std;

void lexTest(string filename);
void graTest(string filename);
void semTest(string filename);
void backTest(string filename);
void modiTest(string filename);

void graInterface();
void modiInterface();

bool FUNC_PREFIX_ENTRY_NAME = false;
bool MIPS_TUPLE_OUTPUT = true;

bool MODIFY = false;
bool DAG_MODIFY = false & MODIFY;

int main()
{
    try{
        //lexTest("lex_test.txt");
        //graTest("sem_test.c0");
        //graTest("example.c0");
        //semTest("example.c0");
        //backTest("back_test.c0");
        //backTest("fab.c0");
        //modiTest("param_num_test.c0");

        //graInterface();
        modiInterface();

        //cout << "Hello world!" << endl;
    }
    catch(string s){
        mylog::fatal << s;
    }
    return 0;
}