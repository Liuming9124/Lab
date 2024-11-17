#include "./sno.h"
#include <sstream>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    int Run, Func, Evals, Dim, NetLength, Explorer, MinerInit, MinerFinal, AdjustmentMax;
    double Rho, mf, mc, Alpha, Beta, Archive;

    stringstream ss;
    for (int i = 1; i < argc; i++)
        ss << argv[i] << " ";
    ss >> Run;
    ss >> Func;
    ss >> Evals;
    ss >> Dim;
    ss >> NetLength;
    ss >> Explorer;
    ss >> MinerInit;
    ss >> MinerFinal;
    ss >> AdjustmentMax;
    ss >> Rho;
    ss >> mf;
    ss >> mc;
    ss >> Alpha;
    ss >> Beta;
    ss >> Archive;

    SNO sno;
    sno.RunALG(Run, Func, Evals, Dim, NetLength, Explorer, MinerInit, MinerFinal, AdjustmentMax, Rho, mf, mc, Alpha, Beta, Archive);
    return 0;
}