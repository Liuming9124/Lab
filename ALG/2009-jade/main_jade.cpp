#include "jade.h"
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
    int Run, NP, G, Dim, Func, Arch;
    double P, C;

    stringstream ss;
    for (int i = 1; i < argc; i++)
        ss << argv[i] << " ";
    ss >> Run;
    ss >> NP;
    ss >> G;
    ss >> Dim;
    ss >> P;
    ss >> C;
    ss >> Arch;
    ss >> Func;

    Jade jade;
    jade.RunALG(Run, NP, G, Dim, P, C, Arch, Func);
    return 0;
}