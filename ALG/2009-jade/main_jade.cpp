#include "jade.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    int Run = atoi(argv[2]);
    int NP = atoi(argv[1]);
    int G = atoi(argv[3]);
    int Bounder = atoi(argv[4]);
    int Dim = atoi(argv[5]);
    int P = atoi(argv[6]);
    int C = atoi(argv[7]);

    Jade jade;
    jade.RunALG( Run, NP,  G, Bounder, Dim, P, C);
    return 0;
}