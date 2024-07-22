#include "de.h"
#include <sstream>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    int Pop , Run, Iter, Dim;
    double Bounder, Cr, F;

    
    stringstream ss;
    for (int i = 1; i < argc; i++)
        ss << argv[i] << " ";
    ss >> Pop;
    ss >> Run;
    ss >> Iter;
    ss >> Dim;
    ss >> Bounder;
    ss >> Cr;
    ss >> F;

    De de;
    de.RunALG( Pop,  Run,  Iter,  Dim,  Bounder, Cr,  F);
    return 0;
}