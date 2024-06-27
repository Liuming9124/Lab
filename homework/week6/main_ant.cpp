#include "antTsp.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main(int argc, char *argv[]){
    string InputName = argv[1];
    int Run = atoi(argv[2]);
    int Iter = atoi(argv[4]);
    int NumAnts = atoi(argv[3]);
    int Alpha = atoi(argv[5]);
    int Beta = atoi(argv[6]);
    int Q = atoi(argv[7]);
    double Rho = atoi(argv[8]);
    double Pmute = atoi(argv[9]);

    AntTsp ant;
    ant.RunALG(InputName, Run, Iter, NumAnts, Alpha, Beta, Q, Rho, Pmute);
    return 0;
}
