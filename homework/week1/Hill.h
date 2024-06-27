#ifndef HILL_H
#define HILL_H

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

class Hill : OneMax
{
public:
    void RunALG(int, int, int);

private:
    // Input from Command-line Argument
    int _Bit;
    int _Run;
    int _Iter;

    int _Nfes;
    int _Mnfes;

    std::vector<bool> _Sol;

    void Init();
    void Evaluation();
    void Reset();
};

void Hill::RunALG(int Bit, int Run, int Iter)
{
    _Bit = Bit;
    _Run = Run;
    _Iter = Iter;
    _Mnfes = _Nfes = 0;

    
    Show = AlgPrint(_Run, "onemax", "hill");
    Show.NewShowDataInt(_Iter);
    for (int i = 0; i < _Run; i++){
        Show.clearResult("../result/onemax/hill/onemaxhill_" + to_string(i) + ".txt");
    }

    while (_Run--){
        cout << "-------------------Run" << Run - _Run << "---------------------" << endl;
        Init();
        Evaluation();
        Reset();
    }
    Show.PrintToFile("../result/onemax/hill/onemaxhillAvg.txt");
    cout << "Average NFEs : " << _Mnfes/Run << endl;
}

void Hill::Evaluation(){
    vector<bool> best = _Sol;
    vector<bool> candidate = _Sol;
    bool best_flag = false;
    for (int i=0; i<_Iter; i++){
        if (best_flag == false){
            _Nfes++;
            Transaction(&candidate, _Bit);
            int value = OneMaxProblem(candidate, _Bit);
            if (value > OneMaxProblem(best, _Bit)){
                best = candidate;
                if (value == _Bit){
                    best_flag = true;
                }
            }
            Print( best, _Bit, _Run, "onemax", "hill");
            Show.SetDataInt(OneMaxProblem(best, _Bit), i);
        }
        else {
            for (int j=i; j<_Iter; j++){
                Show.SetDataInt(OneMaxProblem(best, _Bit), j);
            }
            break;
        }
    }
}

void Hill::Reset(){
    _Mnfes += _Nfes;
    _Nfes = 0;
}

void Hill::Init(){
    _Sol.resize(_Bit);
    for (int i=0; i<_Bit; i++){
        _Sol[i] = rand()%2;
    }
}

#endif