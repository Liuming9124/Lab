#ifndef JADE_H
#define JADE_H

#include "./problem.cpp"
#include "../Tool.h"
#include <queue>
#include <vector>
#include <random>
#include <algorithm>
#pragma once

Tool tool;

typedef struct Particle{
    vector<float> _position;
    float _inCR, _inF;
    float _fitness;
} _Particle;

class Jade: Problem{
public:
    void RunALG( int, int, int, int, int, int, int);
    static bool compareFitness(const _Particle& , const _Particle& );

private:
    int _Run;
    int _NP;
    int _Gen;
    int _Bounder;
    float _Cr;
    float _F;
    float _mCR;
    float _mF;
    vector<float> _SF, _SCR;
    int _Dim;
    int _P;
    int _C;

    _Particle _U, _V;
    vector<_Particle> _X, _A;

    void Init();
    void Evaluation();
    void Reset();

    int selectTopPBest(vector<_Particle>& , float);
    void Mutation();
    void Crossover();
    void Selection();

    float Frand();
    void CheckBorder(_Particle);
};

void Jade::RunALG(int Run, int NP, int Gen, int Bounder, int Dim, int P, int C){
    _Run = Run;
    _NP = NP;
    _Gen = Gen;
    _Bounder = Bounder;
    _Dim = Dim;
    _P = P;
    _C = C;

    while (_Run--){
        cout << "-------------------Run" << Run - _Run << "---------------------" << endl;
        Init();
        Evaluation();
        Reset();
    }
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
        _X[i]._fitness = fun1(_X[i]._position, _Dim);
    }
}

void Jade::Evaluation(){
    for (int g=0; g<_Gen; g++){
        _SF.resize(0);
        _SF.resize(0);
        for (int i=0; i<_NP; i++){
            _X[i]._inCR = tool.rand_normal(_mCR, 0.1);
            _X[i]._inF  = tool.rand_cachy(_mF, 0.1);

            int best, r1, r2, flag=0;
            best = selectTopPBest(_X, _P);
            do {
                r1 = tool.rand_int(0,_NP-1);
            } while (r1!=i);
            do {
                r2 = tool.rand_int(0,_NP+_A.size()-1);
                if (r2>=_NP){
                    r2 -=_NP;
                }
            } while (!flag && (r2!=i && r2!=r1));
            
            for (int j=0; j<_Dim; j++){
                float F = _X[i]._inF;
                _V._position[j] = _X[i]._position[j]
                                     + F*(_X[best]._position[j] - _X[i]._position[j]) 
                                     + F*(_X[r1]._position[j]   - (flag==0)?_X[r2]._position[j]:_A[r2]._position[j]);
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
            _U._fitness = fun1(_U._position, _Dim);
            if (_X[i]._fitness > _U._fitness){
                _A.push_back(_X[i]);
                _X[i]._position = _U._position;
                _SCR.push_back(_X[i]._inCR);
                _SF.push_back(_X[i]._inF);
            }
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
        _mF = meanF;
    }
}


void Jade::Reset(){
    _X.clear();
}

void Jade::CheckBorder(_Particle check){
    for (int i = 0; i<_Dim; i++){
        if (!(check._position[i]<=_Bounder&& check._position[i]>=_Bounder)){
            check._position[i] = (rand() % _Bounder*2 - _Bounder);
        }
    }
}


bool Jade::compareFitness(const _Particle& a, const _Particle& b) {
    return a._fitness < b._fitness;
}

int Jade:: selectTopPBest(vector<_Particle>& X, float p) {
    sort(X.begin(), X.end(), compareFitness);
    int place;
    place = p * _NP;
    (place==0)?place=1:place=tool.rand_int(0,place);
    return place;
}

#endif