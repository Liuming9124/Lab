#ifndef JADE_H
#define JADE_H

#include "./problem.cpp"
#include "../AlgPrint.h"
#include "../Tool.h"
#include <queue>
#include <vector>
#include <random>
#include <algorithm>
#pragma once

class Jade: Problem{
public:
    typedef struct Particle{
        vector<long double> _position;
        float _inCR, _inF;
        long double _fitness;
    } _Particle;

    void RunALG( int, int, int, float, int, float, float, int);
    static bool compareFitness(const _Particle& , const _Particle& );

private:
    int _Run;
    int _NP;
    int _Gen;
    float _Bounder;
    float _Cr;
    float _F;
    float _mCR;
    float _mF;
    vector<float> _SF, _SCR;
    int _Dim;
    float _P;
    float _C;

    _Particle _U, _V;
    vector<_Particle> _X, _A;

    void Init();
    void Evaluation();
    void Reset();

    int selectTopPBest(vector<_Particle>& , float);
    void CheckBorder(_Particle&);
    AlgPrint show;
    Tool tool;
    Problem problem;
};

void Jade::RunALG(int Run, int NP, int Gen, float Bounder, int Dim, float P, float C, int Fun){
    _Run = Run;
    _NP = NP;
    _Gen = Gen;
    _Bounder = Bounder;
    _Dim = Dim;
    _P = P;
    _C = C;
    show = AlgPrint(_Run, "./result", "jade");
    show.NewShowDataFloat(_Gen);

    switch(Fun){
        case 1:
            problem.setStrategy(make_unique<Fun1>());
            break;
        case 2:
            problem.setStrategy(make_unique<Fun2>());
            break;
        case 3:
            problem.setStrategy(make_unique<Fun3>());
            break;
        case 4:
            problem.setStrategy(make_unique<Fun4>());
            break;
        case 5:
            problem.setStrategy(make_unique<Fun5>());
            break;
        case 6:
            problem.setStrategy(make_unique<Fun6>());
            break;
        case 7:
            problem.setStrategy(make_unique<Fun7>());
            break;
        case 8:
            problem.setStrategy(make_unique<Fun8>());
            break;
        case 9:
            problem.setStrategy(make_unique<Fun9>());
            break;
        case 10:
            problem.setStrategy(make_unique<Fun10>());
            break;
        case 11:
            problem.setStrategy(make_unique<Fun11>());
            break;
        // case 12:
        //     problem.setStrategy(make_unique<Fun12>());
        //     break;
        default:
            cout << "Error: No such function" << endl;
            return;
    }

    while (_Run--){
        cout << "-------------------Run" << Run - _Run << "---------------------" << endl;
        Init();
        Evaluation();
        Reset();
    }
    show.PrintToFileFloat("./result/result" + to_string(Fun) + ".txt", _Gen);
    cout << "end" << endl;
}

void Jade::Init(){
    _mCR = 0.5;
    _mF = 0.5;
    _A.resize(0);

    _X.resize(_NP);
    int dim = _Dim;
    for (int i=0; i<_NP; i++){
        _X[i]._position.resize(dim);
        for (int j=0; j<dim; j++){
            _X[i]._position[j] = tool.rand_int(-1*_Bounder, _Bounder);
        }
        _X[i]._fitness = problem.executeStrategy(_X[i]._position, _Dim);
    }
    // init var
    _U._position.resize(dim);
    _V._position.resize(dim);
    _U._fitness = _V._fitness = 0;
    _SCR.resize(_NP);
    _SF.resize(_NP);
}

void Jade::Evaluation(){
    for (int g=0; g<_Gen; g++){
        _SCR.clear();
        _SF.clear();
        for (int i=0; i<_NP; i++){
            _X[i]._inCR = tool.rand_normal(_mCR, 0.1);
            _X[i]._inF  = tool.rand_cauchy(_mF, 0.1);

            int best, r1, r2, flag=0;
            best = selectTopPBest(_X, _P);
            do {
                r1 = tool.rand_int(0,_NP-1);
            } while (r1==i);
            do {
                r2 = tool.rand_int(0,(_NP-1)+(_A.size()-1));
                if (r2>=_NP){
                    int checkFlag = 0;
                    for (int j=0; j<_Dim; j++){
                        if (_A[r2-(_NP-1)]._position[j]!=_X[i]._position[j]){
                            checkFlag = 1;
                            break;
                        }
                    }
                    for (int j=0; j<_Dim; j++){
                        if (_A[r2-(_NP-1)]._position[j]!=_X[r1]._position[j]){
                            checkFlag = 1;
                            break;
                        }
                    }
                    if (checkFlag==0){
                        continue;
                    }
                    else{
                        r2 -= (_NP-1);
                        flag = 1;
                        break;
                    }
                }
            } while (r2==i || r2==r1);

            for (int j=0; j<_Dim; j++){
                float F = _X[i]._inF;
                if (flag==0){
                    _V._position[j] = _X[i]._position[j]
                                     + F*(_X[best]._position[j] - _X[i]._position[j]) 
                                     + F*(_X[r1]._position[j]   - _X[r2]._position[j]);

                }else {
                    _V._position[j] = _X[i]._position[j]
                                     + F*(_X[best]._position[j] - _X[i]._position[j]) 
                                     + F*(_X[r1]._position[j]   - _A[r2]._position[j]);

                }
                CheckBorder(_V);
            }
            int jrand = tool.rand_int(0,_Dim-1);
            for (int j=0; j<_Dim; j++){
                if (j == jrand || tool.rand_float(0,1) < _X[i]._inCR){
                    _U._position[j] = _V._position[j];
                }
                else{
                    _U._position[j] = _X[i]._position[j];
                }
            }
            _U._fitness = problem.executeStrategy(_U._position, _Dim);
            if (_X[i]._fitness > _U._fitness){
                _A.push_back(_X[i]);
                _X[i]._position = _U._position;
                _X[i]._fitness  = _U._fitness;
                _SCR.push_back(_X[i]._inCR);
                _SF.push_back(_X[i]._inF);
            }
        }

        while (_A.size()>_NP){
            // randomly remove one element from A
            int remove = tool.rand_int(0, _A.size()-1);
            _A.erase(_A.begin()+remove);
        }

        // mean Scr
        float meanScr = 0;
        for (int t=0; t<_SCR.size(); t++){
            meanScr+=_SCR[t];
        }
        meanScr/=_SCR.size();

        // Lehmer mean
        float meanF, numerator, denominator;
        meanF = numerator = denominator = 0;
        for (int t=0; t<_SF.size(); t++){
            numerator += _SF[t]*_SF[t];
            denominator += _SF[t];
        }
        meanF = numerator/denominator;

        _mCR = (1-_C)*_mCR + _C*meanScr;
        _mF = (1-_C)*_mF + _C*meanF;

        // show data
        sort(_X.begin(), _X.end(), compareFitness);
        show.SetDataFloat(_Run, _X[0]._fitness, g);
    }
}


void Jade::Reset(){
    _X.clear();
    _A.clear();
    _U._position.clear();
    _V._position.clear();
    
}

void Jade::CheckBorder(_Particle& check){
    for (int i = 0; i<_Dim; i++){
        while (check._position[i]<-1*_Bounder){
            check._position[i] = (-1*_Bounder + check._position[i])/2;
        }
        while (check._position[i]>_Bounder){
            check._position[i] = (_Bounder + check._position[i])/2;
        }
    }
}

bool Jade::compareFitness(const _Particle& a, const _Particle& b) {
    return a._fitness < b._fitness;
}

int Jade::selectTopPBest(vector<_Particle>& X, float p) {
    sort(X.begin(), X.end(), compareFitness);
    int place;
    place = p * _NP;
    place=tool.rand_int(0,place);
    return place;
}

#endif