#ifndef EXHAUSTIVE_H
#define EXHAUSTIVE_H

#include "../problem/OneMax.cpp"
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
    std::vector<bool> _Sol;

    void Init();
    void Evaluation();
    void Reset();

    void Addone(vector<bool>*, int);
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
        Init();
        Evaluation();
        Reset();
    }
    cout << "Average NFEs : " << this->mnfes/Run << endl;
}

void Exhaustive::Evaluation(){
    vector<bool> best = this->_Sol;
    vector<bool> candidate = this->_Sol;
    bool best_flag = false;
    for (int i=0; i<this->_Iter && best_flag == false; i++){
        this->nfes++;
        Addone(&candidate, 0);
        int value = OneMaxProblem(candidate, this->_Bit);
        if (value > OneMaxProblem(best, this->_Bit)){
            best = candidate;
            if (value == this->_Bit){
                best_flag = true;
            }
        }
        Print(i, best, this->_Iter_len, this->_Bit, this->_Run, "onemax", "exhaustive");
    }
}

void Exhaustive::Reset(){
    this->mnfes += this->nfes;
    cout << "End with iter : " << this->nfes << endl;
    this->nfes = 0;
    this->_Iter_len = 0;
}

void Exhaustive::Init(){
    this->_Sol.resize(this->_Bit);
    for (int i=0; i<this->_Bit; i++){
        this->_Sol[i] = rand()%2;
    }
    int count = this->_Iter;
    do {
        this->_Iter_len++;
    } while (count/=10);
}


void Exhaustive::Addone(vector<bool> *x, int place){
    int carryout;
    int temp = (*x)[place]+ 1;
    (*x)[place] = temp%2;
    carryout = temp/2;
    if (carryout == 1 && place+1 < this->_Bit){
        Addone(x, place+1);
    }
    else{
        return;
    }
}

#endif