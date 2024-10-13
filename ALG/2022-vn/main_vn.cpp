#include "./vn.h"
#include <sstream>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    int Run, Func, Evals, Dim, NetLength;
    double History;

    stringstream ss;
    for (int i = 1; i < argc; i++)
        ss << argv[i] << " ";
    ss >> Run;
    ss >> Func;
    ss >> Evals;
    ss >> Dim;
    ss >> NetLength;
    ss >> History;

    // VN vn;
    // vn.RunALG(Run, Func, Evals, Dim, NetLength, History);
    return 0;
}