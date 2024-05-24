#ifndef GAOM_H
#define GAOM_H

#include <algorithm>
#include "../problem/OneMax.cpp"

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

    int _Nfes;
    int _Mnfes;

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
    void Selection_Wheel();
    void Selection_Tournament();
    void Crossover();
    void Mutation();

};

void Gaom::RunALG (int Bit, int Run, int Iter, int Pop, int CR, int MR){
    this->_Bit = Bit;
    this->_Run = Run;
    this->_Iter = Iter;
    this->_Pop = Pop;
    this->_Cr = CR;
    this->_Mr = MR;

    this->_Nfes = this->_Mnfes = 0;

    while (this->_Run--){
        cout << "-------------------Run" << Run - this->_Run << "---------------------" << endl;
        Init();
        Evaluation();
        Reset();
    }
    cout << "Average NFEs : " << this->_Mnfes/Run << endl;
}

void Gaom::Evaluation(){
    for (int iter=0; iter<this->_Iter && (OneMaxProblem(this->_Best, this->_Bit)!=this->_Bit); iter++){
        // (iter < this->_Iter/2) ? Selection_Wheel() : Selection_Tournament();
        Selection_Tournament();
        Crossover();
        Mutation();

        
    }
}


void Gaom::Selection_Wheel(){

}

void Gaom::Selection_Tournament(){
    // Randomly select a number of individuals from the population
    int offset = rand()%this->_Pop;
    int found[offset]={0};
    _Offspring.resize(offset);
    for (int i = 0; i < offset; i++){
        bool flag = false;
        while (!flag){
            int temp = rand() % this->_Pop;
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
        if (rand()%100 < this->_Cr){
            int temp = rand() % this->_Bit;
            for (int j = 0; j < this->_Bit; j++){
                if (j >= temp){
                    std::swap(_Offspring[i*2]._InSol[j], _Offspring[i*2+1]._InSol[j]);
                }
                else{
                    std::swap(_Offspring[i*2]._InSol[j], _Offspring[i*2+1]._InSol[j]);
                }
            }
            _Offspring[i*2]._InFit = OneMaxProblem(_Offspring[i*2]._InSol, this->_Bit);
            _Offspring[i*2+1]._InFit = OneMaxProblem(_Offspring[i*2+1]._InSol, this->_Bit);
        }
    }
}
void Gaom::Mutation(){
    for (int i=0; i < _Offspring.size(); i++){
        if (rand()%100 < this->_Mr){
            int temp = rand() % this->_Bit;
            _Offspring[i]._InSol[temp] = !_Offspring[i]._InSol[temp];
            (_Offspring[i]._InSol[temp] == 0) ? _Offspring[i]._InFit-- : _Offspring[i]._InFit++; 
        }
    }
    std::sort(_Offspring.begin(), _Offspring.end(), [](Ind a, Ind b) {return a._InFit > b._InFit;});
}

void Gaom::Init(){
    GenRandSol(&this->_Best, this->_Bit);
    _Parents.resize(this->_Pop);
    for (int i=0; i<this->_Pop; i++){
        GenRandSol(&_Parents[i]._InSol, this->_Bit);
        _Parents[i]._InFit = OneMaxProblem(_Parents[i]._InSol, this->_Bit);
    }
}

void Gaom::Reset(){
    this->_Mnfes += this->_Nfes;
    this->_Nfes = 0;
    this->_Best.clear();
    this->_Parents.clear();
}

#endif