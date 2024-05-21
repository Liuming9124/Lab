#ifndef EXHAUSTIVE_H
#define EXHAUSTIVE_H

#include "OneMax.cpp"
#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;
using std::setw;

class Exhaustive: OneMax
{
public:
    void RunALG(int, int, int, double);

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

    void Adder(vector<int>*, int);
};

void Exhaustive::RunALG(int Bit, int Run, int Iter, double rate)
{
    this->_Bit = Bit;
    this->_Run = Run;
    this->_Iter = Iter;
    // this->rate = rate;
    this->nfes = this->mnfes = 0;

    while (this->_Run--){
        cout << "-------------------Run" << Run - this->_Run << "---------------------" << endl;
        vector<int> sol = Init();
        Evaluation(sol);
        Reset();
    }
    cout << "Average NFEs : " << this->mnfes/Run << endl;
}

void Exhaustive::Evaluation(vector<int> sol){
    vector<int> best = sol;
    vector<int> candidate = sol;
    bool best_flag = false;
    for (int i=0; i<this->_Iter; i++){
        this->nfes++;
        Adder(&candidate, 1);
        int value = OneMaxProblem(candidate, this->_Bit);
        if (value > OneMaxProblem(best, this->_Bit)){
            best = candidate;
            Print(i, best, this->_Iter_len, this->_Bit, this->_Run, "exhaustive");
            if (value == this->_Bit){
                cout << "Best Solution Found before " << this->_Iter << endl;
                best_flag = true;
                break;
            }
        }
    }
    if (!best_flag){
        Print(this->_Iter, best, this->_Iter_len, this->_Bit, this->_Run, "exhaustive");
    }
}

void Exhaustive::Reset(){
    this->mnfes += this->nfes;
    cout << this->nfes << endl;
    this->nfes = 0;
    this->_Iter_len = 0;
}

vector<int> Exhaustive::Init(){
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


void Exhaustive::Adder(vector<int> *x, int y){
    int carry = y;
    int i = 0;
    while(1){
        int temp = (*x)[i]+ carry;
        (*x)[i] = temp%2;
        carry = temp/2;
        if (carry==1 && i==(*x).size()-1){
            (*x).push_back(1);
            break;
        }
        else if (carry==0){
            break;
        }
        i++;
    }
}

#endif