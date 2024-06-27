#include "Exhaustive.h"
#include <iostream>

int main(int argc, char *argv[])
{

    int Bit = atoi(argv[1]);
    int Run = atoi(argv[2]);     // 執行次數
    int Iter = atoi(argv[3]);    // 世代數

    Exhaustive alg_exhaustive;
    alg_exhaustive.RunALG(Bit, Run, Iter);
}