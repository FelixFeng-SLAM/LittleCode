#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    ifstream in("../PDprocess/test/1.txt");
    string s;
    while(getline(in,s))
    cout<<s.c_str()<<endl;

    return 0;
}
