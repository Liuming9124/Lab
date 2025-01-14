#include "./vnmoo.h"
#include <sstream>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    int Run, Evals, NetLength;
    string Func;
    double History;

    stringstream ss;
    for (int i = 1; i < argc; i++)
        ss << argv[i] << " ";
    ss >> Run;
    ss >> Evals;
    ss >> Func;
    ss >> NetLength;
    ss >> History;

    VNMOO vnmoo;
    vnmoo.RunALG(Run, Evals, Func, NetLength, History);
    return 0;
}