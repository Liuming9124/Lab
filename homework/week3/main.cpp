#include "tsde.h"
#include "tsom.h"
#include "gaom.h"
#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{

    int Bit = atoi(argv[1]);
    int Run = atoi(argv[2]);     // 執行次數
    int Iter = atoi(argv[3]);    // 世代數
    int Tweaks = atoi(argv[4]);  // 鄰近解的數量
    int Tblen = atoi(argv[5]);   // tabu list length
    
    // Tsde alg_tb_deception;
    // alg_tb_deception.RunALG(Bit, Run, Iter, Tweaks, Tblen);

    Tsom alg_tb_onemax;
    alg_tb_onemax.RunALG(Bit, Run, Iter, Tweaks, Tblen);

    // Gaom alg_ga_onemax;
    // alg_ga_onemax.RunALG(Bit, Run, Iter);

}