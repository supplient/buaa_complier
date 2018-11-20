#include <iostream>
#include <string>

using namespace std;

void graTest(string filename);

void graInterface()
{
    string filename;
    cout << "Input filename: ";
    cin >> filename;
    graTest(filename);
}
