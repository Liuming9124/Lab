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
        Transaction(&candidate, this->_Bit);
        int value = OneMaxProblem(candidate, this->_Bit);
        if (value > OneMaxProblem(best, this->_Bit)){
            best = candidate;
            Print(i, best, this->_Iter_len, this->_Bit, this->_Run, "hill");
            if (value == this->_Bit){
                cout << "Best Solution Found before " << this->_Iter << endl;
                best_flag = true;
                break;
            }
        }
    }
    if (!best_flag){
        Print(this->_Iter, best, this->_Iter_len, this->_Bit, this->_Run, "hill");
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

#endif