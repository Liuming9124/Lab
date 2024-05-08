#ifndef HILL_H
#define HILL_H

#include "OneMax.cpp"
#include <iostream>
#include <vector>
using namespace std;

class Hill : OneMax
{
public:
    void RunALG(int, int, int, double);


private:
    // Input from Command-line Argument
    int Bit;
    int Run;
    int Iter;
    double rate;

    // calculate the evaluation
    int nfes;
    int mnfes;

    void Evaluation(vector<int>, int &);

    void Reset();
    void Init();
};

void Hill::RunALG(int Bit, int Run, int Iter, double rate)
{
    this->Bit = Bit;
    this->Run = Run;
    this->Iter = Iter;
    this->rate = rate;

    for (int i = 0; i < Run; i++)
    {
        Reset();
        Init();
        for (int j = 0; j < Iter; j++)
        {
            vector<int> sol(Bit);
            Evaluation(sol, nfes);
        }
    }
}

void Hill::Evaluation(vector<int> sol, int &){

}

void Hill::Reset(){
    
}

void Hill::Init(){

}

#endif