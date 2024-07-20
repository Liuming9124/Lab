#ifndef TOOL_H
#define TOOL_H

#include <vector>
#include <iostream>
#include <random>
using namespace std;

class Tool{
public:
    Tool() : gen(random_device{}()) {} 

    double rand_normal(const double mean, const double stddev){ // normal_distribution
        normal_distribution<> distr(mean, stddev);
        return (distr(gen));
    }

    double rand_cauchy(const double a, const double b){
        cauchy_distribution<> distr(a, b);
        return distr(gen);
    }
    
    double rand_double(double min, double max){
        uniform_real_distribution<double> dis(min, max);
        return dis(gen);
    }

    int rand_int(int min, int max){
        uniform_int_distribution<int> dis(min, max);
        return dis(gen);
    }

private:
    mt19937 gen;
};

#endif