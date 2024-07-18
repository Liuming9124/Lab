#ifndef TOOL_H
#define TOOL_H

#include <vector>
#include <iostream>
#include <random>
using namespace std;
#pragma once

class Tool{
public:
    Tool() : gen(random_device{}()) {} 

    double rand_normal(const double mean, const double stddev){ // normal_distribution
        normal_distribution<> distr(mean, stddev);
        return (distr(gen));
    }

    double rand_cachy(const double a, const double b){
        std::cauchy_distribution<> distr(a, b);
        return distr(gen);
    }
    
    float rand_float(float min, float max){
        std::random_device rd;
        std::uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }

    int rand_int(int min, int max){
        std::random_device rd;
        std::uniform_int_distribution<int> dis(min, max);
        return dis(gen);
    }

private:
    mt19937 gen;
};

#endif