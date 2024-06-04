#include "psoack.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    int Pop = atoi(argv[1]);
    int Run = atoi(argv[2]);
    int Iter = atoi(argv[3]);
    int Dim = atoi(argv[4]);
    int Bounder = atoi(argv[5]);


        
    Psoack pso;
    pso.RunALG(Pop, Run, Iter, Dim, Bounder);
    return 0;
}