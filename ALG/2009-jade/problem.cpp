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

    long double fun1(std::vector<float> xx, int dim){
        long double num=0;
        for (int i=1; i<=dim; i++){
            num += xx[i]*xx[i];
        }
        return num;
    }
    
    long double fun2(std::vector<float> xx, int dim){
        long double num=0;
        // todo
        
        return num;
    }

    
    long double fun3(std::vector<float> xx, int dim){
        long double num=0;
        // todo
        
        return num;
    }

private:
};