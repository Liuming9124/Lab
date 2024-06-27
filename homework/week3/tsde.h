#ifndef TSDE_H
#define TSDE_H

#include "../problem/Deception.cpp"
#include "../problem/AlgPrint.h"
#include <queue>

AlgPrint Show;

class Tsde: Deception {
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

    std::vector<int> _Sol;
    std::vector<int> _Best;
    std::queue<std::vector<int>> _Tblist;
    std::vector<int> _End_value;


    void Init();
    void Evaluation();
    void Reset();

    bool Inqueue(const std::vector<int>&);
    std::vector<int> TweakCp();

};

void Tsde::RunALG (int Bit, int Run, int Iter, int Tweaks, int Tblen){
    _Bit = Bit;
    _Run = Run;
    _Iter = Iter;
    _Tweaks = Tweaks;
    _Tblen = Tblen;
    _Nfes = _Mnfes = 0;

    
    Show = AlgPrint(_Run, "deception", "ts");
    Show.NewShowDataInt(_Iter);
    for (int i = 0; i < _Run; i++){
        Show.clearResult("../result/deception/ts/deceptionts_" + to_string(i) + ".txt");
    }

    while (_Run--){
        cout << "-------------------Run" << Run - _Run << "---------------------" << endl;
        Init();
        Evaluation();
        Reset();
    }
    Show.PrintToFile("../result/deception/ts/deceptiontsAvg.txt");
    cout << "Average NFEs : " << _Mnfes/Run << endl;
}


void Tsde::Evaluation(){
    for (int iter=0; iter<_Iter; iter++){
        if (_Best!=_End_value){
            if (_Tblist.size() > _Tblen){
                _Tblist.pop();
            }
            std::vector<int> R = TweakCp();
            _Nfes++;
            for (int i=0; i<_Tweaks-1; i++){
                _Nfes++;
                std::vector<int> W = TweakCp();

                if ( !Inqueue(W) && ( DeceptionProblemCompare(W, R) || Inqueue(R)) ){
                    R = W;
                }
            }
            if (!Inqueue(R)){
                _Sol = R;
                _Tblist.push(R);
            }
            if (DeceptionProblemCompare(_Sol, _Best)){
                _Best = _Sol;
            }
            Print(iter, _Best, _Bit, _Run, "deception", "ts");
            int temp = 0;
            for (int x:DeceptionProblem(_Best)){
                temp = temp*10 + x;
            }
            Show.SetDataInt(temp, iter);
        }
        else{
            for (int j=iter; j<_Iter; j++){
                int temp = 0;
                for (int x:DeceptionProblem(_Best)){
                    temp = temp*10 + x;
                }
                Show.SetDataInt(temp, iter);
            }
        }
    }
}

void Tsde::Init(){
    _Best.resize(_Bit);
    _Sol.resize(_Bit);
    for (int i=0; i<_Bit; i++){
        _Best[i] = rand()%2;
    }
    _Sol = _Best;
    _Tblist.push(_Best);
    _End_value = GetEndValue(_Bit);
}

void Tsde::Reset(){
    _Mnfes += _Nfes;
    _Nfes = 0;
    _Sol.clear();
    _Best.clear();
    _Tblist = std::queue<std::vector<int>>();
    _End_value.clear();
}

std::vector<int> Tsde::TweakCp(){
    std::vector<int> sol(_Bit);
    sol = _Sol;
    Transaction( &sol, _Bit);
    return sol;
}

bool Tsde::Inqueue(const std::vector<int> &check){
    std::queue<std::vector<int>> tempQueue = _Tblist;
    while (!tempQueue.empty()) {
        if (tempQueue.front() == check) {
            return true;
        }
        tempQueue.pop();  // Remove the front element
    }
    return false;
}

#endif