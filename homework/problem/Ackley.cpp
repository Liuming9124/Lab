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
    float AckleyProblem(int x){
        float xfor2, xfor1;
        
        for (int i=1; i<=_Dim; i++){
            xfor2 += pow(i, 2);
            xfor1 += cos(_C*i);
        }

        return (-_A)*exp(-_B*sqrt( (1/_Dim) * xfor2) - exp( (1/_Dim) * xfor1 )) + _A + exp(1);
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
private:
    int _Dim;
    int _A;
    int _B;
    int _C;
};