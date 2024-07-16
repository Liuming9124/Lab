#include <vector>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <string>
using namespace std;
#pragma once

class Problem
{
public:
    float AckleyProblem(std::vector<float> xx){
        float sum1, sum2;
        
        for (int i=1; i<=_Dim; i++){
            sum1 += pow(xx[i], 2);
            sum2 += cos(_C*xx[i]);
        }
        float term1 = (-_A)*exp(-_B*sqrt( sum1/_Dim));
        float term2 = -exp( sum2/_Dim );

        return term1 + term2 + _A + exp(1);
    }

    float fun1(std::vector<float> xx){
        float num=0;
        for (int i=1; i<=_Dim; i++){
            num += xx[i]*xx[i];
        }
        return num;
    }
    
    float fun2(std::vector<float> xx){
        float num=0;
        // todo
        
        return num;
    }

    void setArgs(float A, float B, float C, int Dim){
        _A = A;
        _B = B;
        _C = C;
        _Dim = Dim;
    }


    // get args
    int getDim(){
        return _Dim;
    }

private:
    int _Dim;
    float _A;
    float _B;
    float _C;
};