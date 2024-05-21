#include "Sa.h"
#include "Saom.h"
#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{

    int Bit = atoi(argv[1]);
    int Run = atoi(argv[2]);     // 執行次數
    int Iter = atoi(argv[3]);    // 世代數
    double Temp = atof(argv[4]); // 溫度
    double Rate = atof(argv[5]); // 降溫速率
    
    Sa alg_sa_deception;
    alg_sa_deception.RunALG(Bit, Run, Iter, Temp, Rate);

    Saom alg_sa_onemax;
    alg_sa_onemax.RunALG(Bit, Run, Iter, Temp, Rate);


}