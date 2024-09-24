#include <vector>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <string>
#include <memory>
#include "./cec14_test_func.cpp"
#include "../../Tool.h"
using namespace std;


#include <WINDOWS.H>    
#include <stdio.h>
#include <math.h>
#include <malloc.h>


void cec14_test_func(double *, double *,int,int,int);

double *OShift,*M,*y,*z,*x_bound;
int ini_flag=0,n_flag,func_flag,*SS;



class Problem
{
public:
    void setStrategy(int func) {
        this->strategy = func;
    }
    
    double executeStrategy(const vector<double>& xx, int dim) const {
        double f;
        cec14_test_func( const_cast<double*>(&xx[0]), &f, dim, 1, strategy);
        return f;
    }

    double getBounderMin() const {
        return -50.0;
    }

    double getBounderMax() const {
        return 50.0;
    }

private:
    int strategy;

};