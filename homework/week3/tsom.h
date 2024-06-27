#ifndef TSOM_H
#define TSOM_H

#include "../problem/OneMax.cpp"
#include "../problem/AlgPrint.h"
#include <queue>

AlgPrint Show;

class Tsom: OneMax {
public:
    void RunALG(int, int, int, int, int);

private:
    // Input from Command-line Argument
    int _Bit;
    int _Run;
    int _Iter;
    int _Tweaks;
    int _Tblen;

    int _Nfes;
    int _Mnfes;

    std::vector<bool> _Sol;
    std::vector<bool> _Best;
    std::queue<std::vector<bool>> _Tblist;


    void Init();
    void Evaluation();
    void Reset();

    bool Inqueue(const std::vector<bool>&);
    std::vector<bool> TweakCp();

};

void Tsom::RunALG (int Bit, int Run, int Iter, int Tweaks, int Tblen){
    _Bit = Bit;
    _Run = Run;
    _Iter = Iter;
    _Tweaks = Tweaks;
    _Tblen = Tblen;
    _Nfes = _Mnfes = 0;


    Show = AlgPrint(_Run, "onemax", "ts");
    Show.NewShowDataInt(_Iter);
    for (int i = 0; i < _Run; i++){
        Show.clearResult("../result/onemax/ts/onemaxts_" + to_string(i) + ".txt");
    }

    while (_Run--){
        cout << "-------------------Run" << Run - _Run << "---------------------" << endl;
        Init();
        Evaluation();
        Reset();
    }
    Show.PrintToFile("../result/onemax/ts/onemaxtsAvg.txt");
    cout << "Average NFEs : " << _Mnfes/Run << endl;
}


void Tsom::Evaluation(){
    for (int iter=0; iter<_Iter; iter++){
        if (OneMaxProblem(_Best, _Bit)!=_Bit){
            if (_Tblist.size() > _Tblen){
                _Tblist.pop();
            }
            std::vector<bool> R = TweakCp();
            _Nfes++;
            for (int i=0; i<_Tweaks-1; i++){
                _Nfes++;
                std::vector<bool> W = TweakCp();

                if ( !Inqueue(W) && ( OneMaxCompare(W, R, _Bit) || Inqueue(R)) ){
                    R = W;
                }
            }
            if (!Inqueue(R)){
                _Sol = R;
                _Tblist.push(R);
            }
            if (OneMaxCompare(_Sol, _Best, _Bit)){
                _Best = _Sol;
            }
            Print( _Best, _Bit, _Run, "onemax", "ts");
        }
        else{
            for (int j=iter; j<_Iter; j++){
                Show.SetDataInt(OneMaxProblem( _Best, _Bit), j);
            }
            break;
        }
    }
}

void Tsom::Init(){
    _Best.resize(_Bit);
    _Sol.resize(_Bit);
    for (int i=0; i<_Bit; i++){
        _Best[i] = rand()%2;
    }
    _Sol = _Best;
    _Tblist.push(_Best);
}

void Tsom::Reset(){
    _Mnfes += _Nfes;
    _Nfes = 0;
    _Sol.clear();
    _Best.clear();
    _Tblist = std::queue<std::vector<bool>>();
}

std::vector<bool> Tsom::TweakCp(){
    std::vector<bool> sol(_Bit);
    sol = _Sol;
    Transaction( &sol, _Bit);
    return sol;
}

bool Tsom::Inqueue(const std::vector<bool> &check){
    std::queue<std::vector<bool>> tempQueue = _Tblist;
    while (!tempQueue.empty()) {
        if (tempQueue.front() == check) {
            return true;
        }
        tempQueue.pop();  // Remove the front element
    }
    return false;
}

#endif