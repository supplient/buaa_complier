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

int main()
{
    try{
        //lexTest("lex_test.txt");
        //graTest("sem_test.c0");
        //graTest("example.c0");
        //semTest("example.c0");
        //backTest("back_test.c0");
        //backTest("example.c0");
        modiTest("modi_test.c0");

        //graInterface();

        //cout << "Hello world!" << endl;
    }
    catch(string s){
        mylog::fatal << s;
    }
    return 0;
}

/*

int i;

int c[100];
int ci;

void fb(){

}

void printchar(char c)
{
    char k;
    k = c;
    cout << k;
}

int loop_condition()
{
    const int two=2;
    int temp;

    temp = i/two;
    if(temp*two != i)
        return(0);
    return(1);
}

void loop_body()
{
    c[ci] = i;
    ci = ci + 1;
    i = i / 2;
}

int main()
{
    cin >> i;
    if(i<0)
        i=-i;
    if(i==0)
    {
        i=i+1;
    }

    ci = 0;
    while(loop_condition())
        loop_body();

    while(ci>0)
    {
        ci = ci - 1;
        cout << "    " << c[ci];
    }

    printchar('D');
    printchar('o');
    printchar('n');
    printchar('e');
    cout << "    +";


    return 0;
}
*/
