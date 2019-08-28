#include <iostream>
#include <random>
#include <fstream>
#include <ctime>

using namespace std;


const char* PITCHNAME[] = {
    "_C",  "_D", "_E",  "_F", "_G", "_A", "_B",
    "C", "D", "E", "F", "G", "A", "B",
    "C_", "D_", "E_", "F_", "G_", "A_", "B_"
};

int main(int argc, char *argv[])
{

    int rows = 100;

    ofstream outFile("pitchnameTest.txt");
    outFile << "This is a pitch name test!" << endl <<endl;

    default_random_engine e(time(0));
    uniform_int_distribution<unsigned> u(0, 20);

    for(int k=0; k<rows; k++){

        for (size_t i = 0; i < 4; ++i){
            outFile << PITCHNAME[u(e)] << "     ";
        }
        outFile << endl << endl <<endl;
    }

    outFile.close();

    return 0;
}
