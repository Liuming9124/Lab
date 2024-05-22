#include <vector>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;
using std::setw;
#pragma once

class OneMax
{
public:
    int OneMaxProblem(std::vector<bool> sol, int bit_size){
        int sum = 0;
        for (int i = 0; i < bit_size; i++){
            sum += sol[i];
        }
        return sum;
    }
    
    void Transaction(std::vector<bool>* sol, int bit_size){
        int index = rand() % bit_size;
        (*sol)[index] = !(*sol)[index];
    }

    void Print(int iter, vector<bool> show, int iter_len, int bit_size, int run, string folder, string alg){
        // TODO: Write append without clear
        std::string filename = "../result/"+ folder + "/" + alg + "/" + folder + alg + "_" + std::to_string(run) + ".txt";
        std::ofstream file(filename, std::ios_base::app);

        if (file.is_open()) {
            // debug mode
            
            // file << "Iter " << std::setw(10) << iter << " : ";
            // for (int x : show) {
            //     file << x;
            // }
            // file << ", Value : " << OneMaxProblem(show, bit_size) << std::endl;

            // chart mode
            file << OneMaxProblem(show, bit_size) << std::endl;
        }
        else {
            std::cerr << "Unable to open file!\n";
        }
    }

    bool OneMaxCompare(std::vector<bool> sol, std::vector<bool> best, int bit_size){
        return OneMaxProblem(sol, bit_size) > OneMaxProblem(best, bit_size);
    }
};