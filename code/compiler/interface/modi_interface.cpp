#include <iostream>
#include <string>

using namespace std;

void modiTest(string filename);

void modiInterface()
{
    string filename;
    cout << "Input filename: ";
    cin >> filename;
    modiTest(filename);
}
