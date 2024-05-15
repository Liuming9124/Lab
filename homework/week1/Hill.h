#ifndef HILL_H
#define HILL_H

#include "OneMax.cpp"
#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;
using std::setw;

class Hill : OneMax
{
public:
    void RunALG(int, int, int, int);

private:
    // Input from Command-line Argument
    int _Bit;
    int _Run;
    int _Iter;
    // double _rate;

    int nfes;
    int mnfes;

    int _Iter_len = 0;

    vector<int> Init();
    void Evaluation(vector<int>);
    void Reset();

    void transaction(vector<int>*);
    void Print(int iter, vector<int>);
};

void Hill::RunALG(int Bit, int Run, int Iter, int Rate)
{
    this->_Bit = Bit;
    this->_Run = Run;
    this->_Iter = Iter;

    while (this->_Run--){
        cout << "-------------------Run" << Run - this->_Run << "---------------------" << endl;
        vector<int> sol = Init();
        Evaluation(sol);
        Reset();
    }
    cout << "Average NFEs : " << this->mnfes/Run << endl;
}

void Hill::Evaluation(vector<int> sol){
    vector<int> best = sol;
    vector<int> candidate = sol;
    bool best_flag = false;
    for (int i=0; i<this->_Iter; i++){
        this->nfes++;
        transaction(&candidate);
        int value = OneMaxProblem(candidate, this->_Bit);
        if (value > OneMaxProblem(best, this->_Bit)){
            best = candidate;
            Print(i, best);
            if (value == this->_Bit){
                cout << "Best Solution Found before " << this->_Iter << endl;
                best_flag = true;
                break;
            }
        }
    }
    if (!best_flag){
        Print(this->_Iter, best);
    }
}

void Hill::Reset(){
    this->mnfes += this->nfes;
    cout << this->nfes << endl;
    this->nfes = 0;
    this->_Iter_len = 0;
}

vector<int> Hill::Init(){
    vector<int> sol(this->_Bit);
    for (int i=0; i<this->_Bit; i++){
        sol[i] = rand()%2;
    }
    int count = this->_Iter;
    do {
        this->_Iter_len++;
    } while (count/=10);
    return sol;
}


void Hill::transaction(vector<int>* sol){
    int index = rand() % this->_Bit;
    (*sol)[index] = !(*sol)[index];
}

void Hill::Print(int iter, vector<int> show){
    cout << "Iter " << std::setw(this->_Iter_len) << iter << " : ";
    for (int x : show){
        cout << x;
    }
    cout << ", Value : " << OneMaxProblem(show, this->_Bit) << endl;

    std::string filename = "../result/hill/hill_" + std::to_string(this->_Run) + ".txt";
    std::ofstream file(filename, std::ios_base::app);

    if (file.is_open()) {
        file << "Iter " << std::setw(10) << iter << " : ";
        for (int x : show) {
            file << x;
        }
        file << ", Value : " << OneMaxProblem(show, this->_Bit) << std::endl;
    }
    else {
        std::cerr << "Unable to open file!\n";
    }
}

#endif