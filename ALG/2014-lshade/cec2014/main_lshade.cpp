#include "lshade2014.h"
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
    int Run, Func, NP, FESS, Dim, Arch, H, NPmin;

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
    ss >> NPmin;

    Lshade lshade;
    lshade.RunALG(Run, Func, NP, FESS, Dim, Arch, H, NPmin);
    return 0;
}