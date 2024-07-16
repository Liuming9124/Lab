#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <random>
using namespace std;
using std::setw;
#pragma once

class Tool{
public:
    Tool(){}

    double generateCR(double meanCR, double stddev) {
        std::random_device rd;
        std::mt19937 gen(rd());

        std::normal_distribution<> d(meanCR, stddev);

        return d(gen);
    }

    double generateRandomNumber(const double mean, const double stddev){
        // Uncomment to use a non-deterministic seed
        // random_device rd;
        // mt19937 gen(rd());
        mt19937 gen(1701);

        normal_distribution<> distr(mean, stddev);

        // Generate a single random number following the normal distribution
        return distr(gen);
    }

private:

};
