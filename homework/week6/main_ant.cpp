#include "antTsp.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// example input:
// .\a.exe tsp.txt 5 100 50 2 2 0.2 1 > ./test.txt

int main(int argc, char *argv[]){
    string InputName = argv[1];
    int Run = atoi(argv[2]);
    int Iter = atoi(argv[4]);
    int NumAnts = atoi(argv[3]);
    int Alpha = atoi(argv[5]);
    int Beta = atoi(argv[6]);
    int Rho = atoi(argv[7]);
    int Q = atoi(argv[8]);

    AntTsp ant;
    ant.RunALG(InputName, Run, Iter, NumAnts, Alpha, Beta, Rho, Q);
    return 0;
}
