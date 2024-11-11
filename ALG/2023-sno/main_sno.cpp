#include "./sno.h"
#include <sstream>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    int Run, Func, Evals, Dim, NetLength;

    stringstream ss;
    for (int i = 1; i < argc; i++)
        ss << argv[i] << " ";
    ss >> Run;
    ss >> Func;
    ss >> Evals;
    ss >> Dim;
    ss >> NetLength;

    SNO sno;
    // sno.RunALG(Run, Func, Evals, Dim, NetLength);
    return 0;
}