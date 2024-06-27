#include "gaom.h"
#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{

    int Bit = atoi(argv[1]);
    int Run = atoi(argv[2]);     // 執行次數
    int Iter = atoi(argv[3]);    // 世代數
    int Pop = atoi(argv[4]);     // 每一世代的族群數
    int CR = atoi(argv[5]);      // 交配率
    int MR = atoi(argv[6]);      // 突變率

    Gaom alg_ga_onemax;
    alg_ga_onemax.RunALG(Bit, Run, Iter, Pop, CR, MR);
}