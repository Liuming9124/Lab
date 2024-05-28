#include <vector>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <string>
using namespace std;
using std::setw;
#pragma once

class Ackley
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
    
    void Print(int iter, vector<bool> show, int iter_len, int bit_size, int run, string folder, string alg){
        // TODO: Write append without clear
        std::string filename = "../result/"+ folder + "/" + alg + "/" + folder + alg + "_" + std::to_string(run) + ".txt";
        std::ofstream file(filename, std::ios_base::app);

        if (file.is_open()) {
            // file << AckleyProblem() << std::endl;
        }
        else {
            std::cerr << "Unable to open file!\n";
        }
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