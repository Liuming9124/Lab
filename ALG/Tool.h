#ifndef TOOL_H
#define TOOL_H

#include <vector>
#include <iostream>
#include <random>
using namespace std;
#pragma once

class Tool{
public:
    Tool(){}

    double rand_normal(const double mean, const double stddev){ // normal_distribution
        mt19937 gen(1701);
        normal_distribution<> distr(mean, stddev);
        return (distr(gen));
    }

    double rand_cachy(const double a, const double b){
        std::mt19937 gen(1701);
        std::cauchy_distribution<> distr(a, b);
        return distr(gen);
    }
    
    float rand_float(float min, float max){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }

    int rand_int(int min, int max){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(min, max);
        return dis(gen);
    }

private:

};

#endif