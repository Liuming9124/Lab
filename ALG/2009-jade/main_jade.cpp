#include "jade.h"
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
    int Run, Func, NP, Fess, Dim, Arch;
    double P, C;

    stringstream ss;
    for (int i = 1; i < argc; i++)
        ss << argv[i] << " ";
    ss >> Run;
    ss >> Func;
    ss >> NP;
    ss >> Fess;
    ss >> Dim;
    ss >> Arch;
    ss >> P;
    ss >> C;

    Jade jade;
    jade.RunALG(Run, Func, NP, Fess, Dim, Arch, P, C);
    return 0;
}