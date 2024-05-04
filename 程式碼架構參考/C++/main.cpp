#include "Alg.h"

int main(int argc, char *argv[])
{
    int Bit = atoi(argv[1]);
    int Run = atoi(argv[2]);     // 執行次數
    int Iter = atoi(argv[3]);    // 世代數
    double rate = atof(argv[4]); // 演算法參數

    Alg algorithm;
    algorithm.RunALG(Bit, Run, Iter, rate);
}