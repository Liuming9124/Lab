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
    AlgPrint(){}
    AlgPrint( int run, const string& folder, const string& alg) : _run(run), _folder(folder), _alg(alg){}
    
    void clearResult(const string& filename) {
        ofstream ofs;
        ofs.open(filename, ofstream::out | ofstream::trunc);
        ofs.close();
    }

    void NewShowDataInt(int amount){
        _iter = amount;
        _dataInt.resize(amount);
        for(int i: _dataInt){
            _dataInt[i] = 0;
        }
    }

    void SetDataInt(int num, int place){
        _dataInt[place] += num;
    }

    void PrintToFile(string fileName){
        // clear old file
        clearResult(fileName);
        // write to file
        std::ofstream file(fileName, std::ios_base::app);
        if (file.is_open()) {
            for(int i: _dataInt){
                file << (double)i/_run << std::endl;
            }
        }
        else {
            std::cerr << "Unable to open file!\n";
        }
    }


private:
    string _folder;
    string _alg;
    int _run;
    int _iter;
    vector<int> _dataInt;
};
