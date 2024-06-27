#include "Hill.h"
#include <iostream>

int main(int argc, char *argv[])
{

    int Bit = atoi(argv[1]);
    int Run = atoi(argv[2]);     // 執行次數
    int Iter = atoi(argv[3]);    // 世代數
    
    Hill alg_hill;
    alg_hill.RunALG(Bit, Run, Iter);

}