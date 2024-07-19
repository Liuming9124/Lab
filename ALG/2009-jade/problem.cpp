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
        for (int i=0; i<dim; i++){
            num += xx[i]*xx[i];
        }
        return num;
    }
    
    long double fun2(std::vector<float> xx, int dim){
        long double num=0;
        for (int i=0; i<dim; i++){
            if (i==0) {
                if (xx[i]>=0){
                    num += xx[i];
                }
                else{
                    num -= xx[i];
                }
                continue;
            }
            if (xx[i]>=0){
                num *= xx[i];
            }
            else{
                num -= xx[i];
            }
        }
        for (int i=0; i<dim; i++){
            if (xx[i]>=0){
                num += xx[i];
            }
            else{
                num -= xx[i];
            }
        } 
        return num;
    }

    
    long double fun3(std::vector<float> xx, int dim){
        long double num=0;
        for (int i=0; i<dim; i++){
            long double temp = 0;
            for (int j=1; j<i; j++){
                temp += xx[j];
            }
            num += temp*temp;
        }
        return num;
    }

    long double fun4(std::vector<float> xx, int dim){
        long double num = xx[0];
        for (int i=1; i<dim; i++){
            if (xx[i]>num){
                num = xx[i];
            }
        }
        return num;
    }

private:
};