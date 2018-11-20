#include <iostream>
#include <string>

using namespace std;

void lexTest(string filename);
void graTest(string filename);
void semTest(string filename);

void graInterface();

int main()
{
    //lexTest("lex_test.txt");
    //graTest("sem_test.c0");
    //graTest("example.c0");
    semTest("sem_test.c0");

    //graInterface();

    //cout << "Hello world!" << endl;
    return 0;
}
