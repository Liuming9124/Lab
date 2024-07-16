#include "jade.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    int NP = atoi(argv[1]);
    int Run = atoi(argv[2]);
    int Iter = atoi(argv[3]);
    int Dim = atoi(argv[4]);
    int Bounder = atoi(argv[5]);
    int Cr = atoi(argv[6]);
    int F = atoi(argv[7]);
    int G = atoi(argv[8]);

    Jade jade;
    jade.RunALG( NP,  Run,  Iter,  Dim,  Bounder, Cr,  F, G);
    return 0;
}