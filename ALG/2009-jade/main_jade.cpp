#include "jade.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    int Run = atoi(argv[1]);
    int NP = atoi(argv[2]);
    int G = atoi(argv[3]);
    float Bounder = atoi(argv[4]);
    int Dim = atoi(argv[5]);
    float P = atoi(argv[6]);
    float C = atoi(argv[7]);
    int Fun = atoi(argv[8]);

    Jade jade;
    jade.RunALG( Run, NP,  G, Bounder, Dim, P, C, Fun);
    return 0;
}