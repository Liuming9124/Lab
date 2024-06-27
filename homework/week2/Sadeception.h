#ifndef SADECEPTION_H
#define SADECEPTION_H

#include "../problem/Deception.cpp"
#include "../problem/AlgPrint.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;
using std::setw;

AlgPrint Show;

class Sa : Deception
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

    int _nfes;
    int _mnfes;

    vector<int> _end_value;

    vector<int> Init();
    void Evaluation(vector<int>);
    void Reset(double);
};

void Sa::RunALG(int Bit, int Run, int Iter, double Temp, double Rate){
    _Bit = Bit;
    _Run = Run;
    _Iter = Iter;
    _Temp = Temp;
    _Rate = Rate;
    _nfes = _mnfes = 0;

    
    Show = AlgPrint(_Run, "deception", "sa");
    Show.NewShowDataInt(_Iter);
    for (int i = 0; i < _Run; i++){
        Show.clearResult("../result/deception/sa/deceptionsa_" + to_string(i) + ".txt");
    }

    while (_Run--){
        cout << "-------------------Run" << Run - _Run << "---------------------" << endl;
        vector<int> sol = Init();
        Evaluation(sol);
        Reset(Temp);
    }
    Show.PrintToFile("../result/deception/sa/deceptionsaAvg.txt");
    cout << "Average _nfes : " << _mnfes/Run << endl;
}

void Sa::Evaluation(vector<int> sol){
    vector<int> best = sol;
    vector<int> candidate = sol;
    bool best_flag = false;
    for (int i=0; i<_Iter; i++){
        if (best_flag ==false){
            _nfes++;
            Transaction(&candidate, _Bit);
            if ( DeceptionProblemCompare(candidate, best) ){
                best = candidate;
                if (_end_value == best){
                    best_flag = true;
                }
            }
            else{
                double p = (rand() % 10000) / 100;
                if (p < _Temp){
                    best = candidate;
                }
            }
            _Temp *= _Rate;
            Print(i, best, _Bit, _Run, "deception", "sa");
            int temp = 0;
            for (int x:DeceptionProblem(best)){
                temp = temp*10 + x;
            }
            Show.SetDataInt(temp, i);
        }
        else{
            for (int j=i; j<_Iter; j++){
                int temp = 0;
                for (int x:DeceptionProblem(best)){
                    temp = temp*10 + x;
                }
                Show.SetDataInt(temp, j);
            }
            break;
        }
    }
}

void Sa::Reset(const double Temp){
    _mnfes += _nfes;
    _nfes = 0;
    _Temp = Temp;
}

vector<int> Sa::Init(){
    vector<int> sol(_Bit);
    for (int i=0; i<_Bit; i++){
        sol[i] = rand()%2;
    }
    _end_value = GetEndValue(_Bit);
    return sol;
}


#endif