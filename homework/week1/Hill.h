#ifndef HILL_H
#define HILL_H

#include "../problem/OneMax.cpp"
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

    int _Nfes;
    int _Mnfes;

    int _Iter_len = 0;
    std::vector<bool> _Sol;

    void Init();
    void Evaluation();
    void Reset();
};

void Hill::RunALG(int Bit, int Run, int Iter, int Rate)
{
    this->_Bit = Bit;
    this->_Run = Run;
    this->_Iter = Iter;
    this->_Mnfes = this->_Nfes = 0;

    while (this->_Run--){
        cout << "-------------------Run" << Run - this->_Run << "---------------------" << endl;
        Init();
        Evaluation();
        Reset();
    }
    cout << "Average NFEs : " << this->_Mnfes/Run << endl;
}

void Hill::Evaluation(){
    vector<bool> best = this->_Sol;
    vector<bool> candidate = this->_Sol;
    bool best_flag = false;
    for (int i=0; i<this->_Iter && best_flag == false; i++){
        this->_Nfes++;
        Transaction(&candidate, this->_Bit);
        int value = OneMaxProblem(candidate, this->_Bit);
        if (value > OneMaxProblem(best, this->_Bit)){
            best = candidate;
            if (value == this->_Bit){
                best_flag = true;
            }
        }
        Print(i, best, this->_Iter_len, this->_Bit, this->_Run, "onemax", "hill");
    }
}

void Hill::Reset(){
    this->_Mnfes += this->_Nfes;
    cout << "End with iter : " << this->_Nfes << endl;
    this->_Nfes = 0;
    this->_Iter_len = 0;
}

void Hill::Init(){
    this->_Sol.resize(this->_Bit);
    for (int i=0; i<this->_Bit; i++){
        this->_Sol[i] = rand()%2;
    }
    int count = this->_Iter;
    do {
        this->_Iter_len++;
    } while (count/=10);
}

#endif