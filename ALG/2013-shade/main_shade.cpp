#include "shade.h"
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
    int Run, Func, NP, FESS, Dim, Arch, H;

    stringstream ss;
    for (int i = 1; i < argc; i++)
        ss << argv[i] << " ";
    ss >> Run;
    ss >> Func;
    ss >> NP;
    ss >> FESS;
    ss >> Dim;
    ss >> Arch;
    ss >> H;

    Shade shade;
    shade.RunALG(Run, Func, NP, FESS, Dim, Arch, H);
    return 0;
}