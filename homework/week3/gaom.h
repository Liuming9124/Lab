#ifndef GAOM_H
#define GAOM_H

#include <algorithm>
#include "../problem/OneMax.cpp"
#include "../problem/AlgPrint.h"

AlgPrint Show;

class Gaom: OneMax {
public:
    void RunALG(int, int, int, int, int, int);

private:
    // Input from Command-line Argument
    int _Bit;
    int _Run;
    int _Iter;
    int _Mr;
    int _Cr;
    int _Pop;

    typedef struct Individual{
        std::vector<bool> _InSol;
        int _InFit;
    } Ind;
    std::vector<Ind> _Parents;
    std::vector<Ind> _Offspring;
    std::vector<bool> _Best;
    
    void Init();
    void Evaluation();
    void Reset();

    void Selection();
    void Crossover();
    void Mutation();

};

void Gaom::RunALG (int Bit, int Run, int Iter, int Pop, int CR, int MR){
    _Bit = Bit;
    _Run = Run;
    _Iter = Iter;
    _Pop = Pop;
    _Cr = CR;
    _Mr = MR;

    Show = AlgPrint(_Run, "onemax", "ga");
    Show.NewShowDataInt(_Iter);
    for (int i = 0; i < _Run; i++){
        Show.clearResult("../result/onemax/ga/onemaxga_" + to_string(i) + ".txt");
    }

    while (_Run--){
        cout << "-------------------Run" << Run - _Run << "---------------------" << endl;
        Init();
        Evaluation();
        Reset();
    }
    Show.PrintToFile("../result/onemax/ga/onemaxgaAvg.txt");
}

void Gaom::Evaluation(){
    for (int iter=0; iter<_Iter; iter++){
        if (OneMaxProblem(_Best, _Bit)!=_Bit){
            Selection();
            Crossover();
            Mutation();

            if (_Offspring[0]._InFit > OneMaxProblem(_Best, _Bit)){
                _Best = _Offspring[0]._InSol;
            }

            // Update Parents
            _Parents.resize(_Pop-_Offspring.size());
            std::copy(_Offspring.begin(), _Offspring.end(), std::back_inserter(_Parents));

            Print(_Best, _Bit, _Run, "onemax", "ga");
            Show.SetDataInt(OneMaxProblem(_Best, _Bit), iter);
        }
        else{
            for (int j=iter; j<_Iter; j++){
                Show.SetDataInt(OneMaxProblem(_Best, _Bit), j);
            }
            break;
        }
    }
}

void Gaom::Selection(){
    // Randomly select a number of individuals from the population
    int offset = rand()%_Pop;
    int found[offset]={0};
    _Offspring.resize(offset);
    for (int i = 0; i < offset; i++){
        bool flag = false;
        while (!flag){
            int temp = rand() % _Pop;
            for (int j : found){
                if (j == temp){
                    continue;
                }
            }
            found[i] = temp;
            break;
        }
        _Offspring[i] = _Parents[found[i]];
    }
    // Leave half of the selected better individuals in the population
    std::sort(_Offspring.begin(), _Offspring.end(), [](Ind a, Ind b) {return a._InFit > b._InFit;});
    (offset%2==0) ? _Offspring.resize(offset/2) : _Offspring.resize(offset/2+1);
}

void Gaom::Crossover(){
    // 單點交配
    for (int i=0; i < _Offspring.size()/2; i++){
        if (rand()%100 < _Cr){
            int temp = rand() % _Bit;
            for (int j = 0; j < _Bit; j++){
                if (j >= temp){
                    std::swap(_Offspring[i*2]._InSol[j], _Offspring[i*2+1]._InSol[j]);
                }
                else{
                    std::swap(_Offspring[i*2]._InSol[j], _Offspring[i*2+1]._InSol[j]);
                }
            }
            _Offspring[i*2]._InFit = OneMaxProblem(_Offspring[i*2]._InSol, _Bit);
            _Offspring[i*2+1]._InFit = OneMaxProblem(_Offspring[i*2+1]._InSol, _Bit);
        }
    }
}
void Gaom::Mutation(){
    for (int i=0; i < _Offspring.size(); i++){
        if (rand()%100 < _Mr){
            int temp = rand() % _Bit;
            _Offspring[i]._InSol[temp] = !_Offspring[i]._InSol[temp];
            (_Offspring[i]._InSol[temp] == 0) ? _Offspring[i]._InFit-- : _Offspring[i]._InFit++; 
        }
    }
    std::sort(_Offspring.begin(), _Offspring.end(), [](Ind a, Ind b) {return a._InFit > b._InFit;});
}

void Gaom::Init(){
    GenRandSol(&_Best, _Bit);
    _Parents.resize(_Pop);
    for (int i=0; i<_Pop; i++){
        GenRandSol(&_Parents[i]._InSol, _Bit);
        _Parents[i]._InFit = OneMaxProblem(_Parents[i]._InSol, _Bit);
    }
}

void Gaom::Reset(){
    _Best.clear();
    _Parents.clear();
}

#endif