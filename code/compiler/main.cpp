#include <iostream>
#include <string>
#include "log.h"

using namespace std;

void lexTest(string filename);
void graTest(string filename);
void semTest(string filename);

void graInterface();

int main()
{
    try{
        //lexTest("lex_test.txt");
        //graTest("sem_test.c0");
        //graTest("example.c0");
        semTest("example.c0");

        //graInterface();

        //cout << "Hello world!" << endl;
    }
    catch(string s){
        log::fatal << s;
    }
    return 0;
}
