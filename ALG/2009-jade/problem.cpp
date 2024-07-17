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

    float fun1(std::vector<float> xx, int dim){
        float num=0;
        for (int i=1; i<=dim; i++){
            num += xx[i]*xx[i];
        }
        return num;
    }
    
    float fun2(std::vector<float> xx, int dim){
        float num=0;
        // todo
        
        return num;
    }

    
    float fun3(std::vector<float> xx, int dim){
        float num=0;
        // todo
        
        return num;
    }

private:
};