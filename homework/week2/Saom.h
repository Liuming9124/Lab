#ifndef SAOM_H
#define SAOM_H

#include "OneMax.cpp"
#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;
using std::setw;

class Saom : OneMax
{
public:
    void RunALG(int, int, int, double, double);

private:
    // Input from Command-line Argument
    int _Bit;
    int _Run;
    int _Iter;
    double _Temp;
    double _Rate;

    int nfes;
    int mnfes;

    int _Iter_len = 0;

    vector<int> Init();
    void Evaluation(vector<int>);
    void Reset(double);
};

void Saom::RunALG(int Bit, int Run, int Iter, double Temp, double Rate){
    this->_Bit = Bit;
    this->_Run = Run;
    this->_Iter = Iter;
    this->_Temp = Temp;
    this->_Rate = Rate;
    this->nfes = this->mnfes = 0;

    while (this->_Run--){
        cout << "-------------------Run" << Run - this->_Run << "---------------------" << endl;
        vector<int> sol = Init();
        Evaluation(sol);
        Reset(Temp);
    }
    cout << "Average NFEs : " << this->mnfes/Run << endl;
}

void Saom::Evaluation(vector<int> sol){
    vector<int> best = sol;
    vector<int> candidate = sol;
    bool best_flag = false;
    for (int i=0; i<this->_Iter; i++){
        this->nfes++;
        Transaction(&candidate, this->_Bit);
        if ( OneMaxProblem(candidate, this->_Bit) > OneMaxProblem(best, this->_Bit)){
            best = candidate;
            Print(i, best, this->_Iter_len, this->_Bit, this->_Run);
            if (OneMaxProblem(candidate, this->_Bit) == this->_Bit){
                cout << "Best Solution Found before " << this->_Iter << endl;
                best_flag = true;
                break;
            }
        }
        else{
            double p = (rand()%10000)/100000.0;
            if (p < this->_Temp){
                best = candidate;
                Print(i, best, this->_Iter_len, this->_Bit, this->_Run);
            }
        }
        this->_Temp *= this->_Rate;
    }
    if (!best_flag){
        Print(this->_Iter, best, this->_Iter_len, this->_Bit, this->_Run);
    }
    
}

void Saom::Reset(const double Temp){
    this->mnfes += this->nfes;
    cout << this->nfes << endl;
    this->nfes = 0;
    this->_Iter_len = 0;
    this->_Temp = Temp;
}

vector<int> Saom::Init(){
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