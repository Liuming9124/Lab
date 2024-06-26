#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;
using std::setw;
#pragma once

class AlgPrint{
public:
    void Print(int iter, std::vector<int> show, int iter_len, int bit_size, int run, string folder, string alg);
    void WriteFile();
private:
    string _folder;
    string _alg;
    int _iter;
    int _iter_len;
    int _run;
};

void AlgPrint::Print(int iter, std::vector<int> show, int iter_len, int bit_size, int run, string folder, string alg){

}

void WriteFile(){

}