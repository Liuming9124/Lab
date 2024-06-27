#ifndef EXHAUSTIVE_H
#define EXHAUSTIVE_H

#include "../problem/OneMax.cpp"
#include "../problem/AlgPrint.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;
using std::setw;

AlgPrint Show;

class Exhaustive: OneMax
{
public:
    void RunALG(int, int, int);

private:
    // Input from Command-line Argument
    int _Bit;
    int _Run;
    int _Iter;

    int nfes;
    int mnfes;

    std::vector<bool> _Sol;

    void Init();
    void Evaluation();
    void Reset();

    void Addone(vector<bool>*, int);
};

void Exhaustive::RunALG(int Bit, int Run, int Iter)
{
    _Bit = Bit;
    _Run = Run;
    _Iter = Iter;
    nfes = mnfes = 0;

    Show = AlgPrint(_Run, "onemax", "exhaustive");
    Show.NewShowDataInt(_Iter);
    for (int i = 0; i < _Run; i++){
        Show.clearResult("../result/onemax/exhaustive/onemaxexhaustive_" + to_string(i) + ".txt");
    }

    while (_Run--){
        cout << "-------------------Run" << Run - _Run << "---------------------" << endl;
        Init();
        Evaluation();
        Reset();
    }
    Show.PrintToFile("../result/onemax/exhaustive/onemaxexhaustiveAvg.txt");
    cout << "Average NFEs : " << mnfes/Run << endl;
}

void Exhaustive::Evaluation(){
    vector<bool> best = _Sol;
    vector<bool> candidate = _Sol;
    bool best_flag = false;
    for (int i=0; i<_Iter; i++){
        if (best_flag == false){
            nfes++;
            Addone(&candidate, 0);
            int value = OneMaxProblem(candidate, _Bit);
            if (value > OneMaxProblem(best, _Bit)){
                best = candidate;
                if (value == _Bit){
                    best_flag = true;
                }
            }
            Print( _Sol, _Bit, _Run, "onemax", "exhaustive");
            Show.SetDataInt(OneMaxProblem(best, _Bit), i);
        }
        else{
            for (int j=i; j<_Iter; j++){
                Show.SetDataInt(OneMaxProblem(best, _Bit), j);
            }
            break;
        }
    }
}

void Exhaustive::Reset(){
    mnfes += nfes;
    nfes = 0;
}

void Exhaustive::Init(){
    _Sol.resize(_Bit);
    for (int i=0; i<_Bit; i++){
        _Sol[i] = rand()%2;
    }
}


void Exhaustive::Addone(vector<bool> *x, int place){
    int carryout;
    int temp = (*x)[place]+ 1;
    (*x)[place] = temp%2;
    carryout = temp/2;
    if (carryout == 1 && place+1 < _Bit){
        Addone(x, place+1);
    }
    else{
        return;
    }
}

#endif