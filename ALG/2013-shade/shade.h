#ifndef SHADE_H
#define SHADE_H

#include "./problem.cpp"
#include "../AlgPrint.h"
#include "../Tool.h"
#include <queue>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

class Shade
{
public:

    void RunALG(int, int, int, int, int, int, int);

private:
    int _Run;
    int _NP;
    int _Gen;
    int _Arch;
    int _Dim;
    int _FESS;
    int _H;
    int _k;
    vector<double> _SF, _SCR;

    typedef struct History
    {
        double _MCR, _MF;
    } _History;
    vector<_History> _HS;

    typedef struct Particle
    {
        vector<double> _position;
        double _inCR, _inF, _inP;
        double _fitness;
        int _index;
    } _Particle;
    _Particle _U, _V;
    vector<_Particle> _X, _A;

    void Init();
    void Evaluation();
    void Reset();

    int selectTopPBest(vector<_Particle>, double);
    void CheckBorder(_Particle &, _Particle &);
    static bool compareFitness(const _Particle &, const _Particle &);

    AlgPrint show;
    Tool tool;
    Problem problem;
};

void Shade::RunALG(int Run, int NP, int FESS, int Dim, int Arch, int H, int Func)
{
    _Run = Run;
    _NP = NP;
    _Gen = FESS / NP;
    _Dim = Dim;
    _Arch = 0;
    _FESS = FESS;
    _H = H;
    if (Arch!=0){
        _Arch = _NP;
    }
    show = AlgPrint(_Run, "./result", "Shade");
    show.NewShowDataDouble(_Gen);

    switch (Func)
    {
    case 0:
        problem.setStrategy(make_unique<FuncAckley>());
        break;
    case 1:
        problem.setStrategy(make_unique<Func1>());
        break;
    case 2:
        problem.setStrategy(make_unique<Func2>());
        break;
    case 3:
        problem.setStrategy(make_unique<Func3>());
        break;
    case 4:
        problem.setStrategy(make_unique<Func4>());
        break;
    case 5:
        problem.setStrategy(make_unique<Func5>());
        break;
    case 6:
        problem.setStrategy(make_unique<Func6>());
        break;
    case 7:
        problem.setStrategy(make_unique<Func7>());
        break;
    case 8:
        problem.setStrategy(make_unique<Func8>());
        break;
    case 9:
        problem.setStrategy(make_unique<Func9>());
        break;
    case 10:
        problem.setStrategy(make_unique<Func10>());
        break;
    case 11:
        problem.setStrategy(make_unique<Func11>());
        break;
    default:
        cout << "Error: No such Funcction" << endl;
        return;
    }

    while (_Run--)
    {
        cout << "-------------------Run" << Run - _Run << "---------------------" << endl;
        Init();
        Evaluation();
        Reset();
    }
    show.PrintToFileDouble("./result/result" + to_string(Func) + ".txt", _Gen);
    cout << "end" << endl;
}

void Shade::Init()
{
    _A.resize(0);
    _X.resize(_NP);
    _k = 0;
    _HS.resize(_H);
    for (int i = 0; i < _H; i++)
    {
        _HS[i]._MCR = 0.5;
        _HS[i]._MF = 0.5;
    }

    int dim = _Dim;
    // random init _X
    for (int i = 0; i < _NP; i++)
    {
        _X[i]._position.resize(dim);
        for (int j = 0; j < dim; j++)
        {
            _X[i]._position[j] = tool.rand_double(problem.getBounderMin(), problem.getBounderMax());
        }
        _X[i]._fitness = problem.executeStrategy(_X[i]._position, _Dim);
        _X[i]._index = i;
        _X[i]._inCR = _X[i]._inF = 0;
    }
    // init var
    _U._position.resize(dim);
    _V._position.resize(dim);
    _U._fitness = _V._fitness = 0;
    _SCR.resize(_NP);
    _SF.resize(_NP);
}

void Shade::Evaluation()
{
    for (int g = 0; g < _Gen; g++)
    {
        vector<double> deltaF; // to store fitness to calculate mean
        deltaF.clear();
        _SCR.clear();
        _SF.clear();
        for (int i = 0; i < _NP; i++)
        {
            // init CR & F & P
            int index = tool.rand_int(0, _H - 1);

            _X[i]._inCR = tool.rand_normal(_HS[index]._MCR, 0.1);
            if (_X[i]._inCR>1){
                _X[i]._inCR = 1;
            }
            else if (_X[i]._inCR<0){
                _X[i]._inCR = 0;
            }
            do
            {
                _X[i]._inF = tool.rand_cauchy(_HS[index]._MF, 0.1);
                if (_X[i]._inF >=1)
                {
                    _X[i]._inF = 1;
                }
            } while (_X[i]._inF <= 0);

            _X[i]._inP = tool.rand_double(2/_NP, 0.2);
            
            // Random choose three place to mutation
            int best, r1, r2, flag = 0;
            best = selectTopPBest(_X, _X[i]._inP);
            do
            {
                r1 = tool.rand_int(0, _NP - 1);
            } while (r1 == i);
            do
            {
                r2 = tool.rand_int(0, _NP + _A.size() - 1);
                if (r2 >= _NP)
                {
                    bool checkFlag = 0;
                    for (int j = 0; j < _Dim; j++)
                    {
                        if (_A[r2 - _NP]._position[j] != _X[i]._position[j])
                        {
                            checkFlag = 1;
                            break;
                        }
                    }
                    for (int j = 0; j < _Dim; j++)
                    {
                        if (_A[r2 - _NP]._position[j] != _X[r1]._position[j])
                        {
                            checkFlag = 1;
                            break;
                        }
                    }
                    if (checkFlag == 0)
                    {
                        continue;
                    }
                    else
                    {
                        r2 -= _NP;
                        flag = 1;
                        break;
                    }
                }
            } while (r2 == i || r2 == r1);

            // mutation & check boundary
            for (int j = 0; j < _Dim; j++)
            {
                double F = _X[i]._inF;
                if (flag == 0)
                {
                    _V._position[j] = _X[i]._position[j] + F * (_X[best]._position[j] - _X[i]._position[j]) + F * (_X[r1]._position[j] - _X[r2]._position[j]);
                }
                else
                {
                    _V._position[j] = _X[i]._position[j] + F * (_X[best]._position[j] - _X[i]._position[j]) + F * (_X[r1]._position[j] - _A[r2]._position[j]);
                }
                CheckBorder(_V, _X[i]);
            }
            // crossover
            int jrand = tool.rand_int(0, _Dim - 1);
            for (int j = 0; j < _Dim; j++)
            {
                if (j == jrand || tool.rand_double(0, 1) < _X[i]._inCR)
                {
                    _U._position[j] = _V._position[j];
                }
                else
                {
                    _U._position[j] = _X[i]._position[j];
                }
            }
            // Selection
            _U._fitness = problem.executeStrategy(_U._position, _Dim);
            if (_X[i]._fitness >= _U._fitness)
            {
                if (_X[i]._fitness > _U._fitness){
                    _A.push_back(_X[i]);
                    _SCR.push_back(_X[i]._inCR);
                    _SF.push_back(_X[i]._inF);
                    deltaF.push_back(_X[i]._fitness - _U._fitness);
                }
                _X[i]._position = _U._position;
                _X[i]._fitness = _U._fitness;
            }
        }

        while (_A.size() > _Arch)
        {
            // randomly remove one element from A
            int remove = tool.rand_int(0, _A.size() - 1);
            _A.erase(_A.begin() + remove);
        }

        if (_SCR.size() != 0 && _SF.size() != 0){
            // prepare param
            double WKdenominator = 0;
            for (int t = 0; t < _SCR.size(); t++)
            {
                WKdenominator += deltaF[t];
            }

            double mCR, mF, numerator, denominator;
            mCR = mF = numerator = denominator = 0;
            for (int t = 0; t < _SCR.size(); t++)
            {
                // mean weight Scr
                mCR += (deltaF[t] / WKdenominator) * _SCR[t];
                // Lehmer mean
                numerator += (deltaF[t] / WKdenominator) * _SF[t] * _SF[t];
                denominator += (deltaF[t] / WKdenominator) * _SF[t];
            }
            mF = numerator / denominator;
            
            _HS[_k]._MCR = mCR;
            _HS[_k]._MF = mF;

            _k++;
            if (_k == _H)
                _k = 0;

        }
        
        // show data
        double tmp = _X[0]._fitness;
        for (int p=1; p<_NP; p++){
            if (tmp > _X[p]._fitness)
                tmp = _X[p]._fitness;
        }
        show.SetDataDouble(_Run, tmp, g);
    }
}

void Shade::Reset()
{
    _X.clear();
    _A.clear();
    _U._position.clear();
    _V._position.clear();
}

void Shade::CheckBorder(_Particle &check, _Particle &old)
{
    for (int i = 0; i < _Dim; i++)
    {
        if (check._position[i] < problem.getBounderMin())
        {
            check._position[i] = (problem.getBounderMin() + old._position[i]) / 2;
        }
        if (check._position[i] > problem.getBounderMax())
        {
            check._position[i] = (problem.getBounderMax() + old._position[i]) / 2;
        }
    }
}

bool Shade::compareFitness(const _Particle &a, const _Particle &b)
{
    return a._fitness < b._fitness;
}

int Shade::selectTopPBest(vector<_Particle> X, double p)
{
    vector<_Particle> tmp = X;
    sort(tmp.begin(), tmp.end(), compareFitness);
    int place;
    place = p * _NP;
    place = tool.rand_int(0, place);
    return tmp[place]._index;
}

#endif