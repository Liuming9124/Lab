#include "shade.h"
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
    int Run, NP, FESS, Dim, Func, Arch, H;

    stringstream ss;
    for (int i = 1; i < argc; i++)
        ss << argv[i] << " ";
    ss >> Run;
    ss >> NP;
    ss >> FESS;
    ss >> Dim;
    ss >> Arch;
    ss >> H;
    ss >> Func;

    Shade shade;
    shade.RunALG(Run, NP, FESS, Dim, Arch, H, Func);
    return 0;
}