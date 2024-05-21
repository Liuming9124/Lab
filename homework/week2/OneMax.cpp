#include <vector>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;
using std::setw;
#pragma once

class OneMax
{
public:
    int OneMaxProblem(std::vector<int> sol, int bit_size){
        int sum = 0;
        for (int i = 0; i < bit_size; i++){
            sum += sol[i];
        }
        return sum;
    }
    
    void Transaction(std::vector<int>* sol, int bit_size){
        int index = rand() % bit_size;
        (*sol)[index] = !(*sol)[index];
    }

    void Print(int iter, vector<int> show, int iter_len, int bit_size, int run){
        cout << "Iter " << std::setw(iter_len) << iter << " : ";
        for (int x : show){
            cout << x;
        }
        cout << ", Value : " << OneMaxProblem(show, bit_size) << endl;

        // TODO: Write append without clear
        std::string filename = "../result/sa/onemax/saom_" + std::to_string(run) + ".txt";
        std::ofstream file(filename, std::ios_base::app);

        if (file.is_open()) {
            file << "Iter " << std::setw(10) << iter << " : ";
            for (int x : show) {
                file << x;
            }
            file << ", Value : " << OneMaxProblem(show, bit_size) << std::endl;
        }
        else {
            std::cerr << "Unable to open file!\n";
        }
    }
};