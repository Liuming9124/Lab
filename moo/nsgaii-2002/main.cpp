#include "./nsgaii.h"
#include <sstream>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    int Run, Func, Evals, Dim, Populatoin;
    double CR, MR;

    stringstream ss;
    for (int i = 1; i < argc; i++)
        ss << argv[i] << " ";
    ss >> Run;
    ss >> Func;
    ss >> Evals;
    ss >> Dim;
    ss >> Populatoin;
    ss >> CR;
    ss >> MR;

    NSGAII nsgaii;
    nsgaii.RunALG(Run, Func, Evals, Dim, Populatoin, CR, MR);
    return 0;
}