#ifndef EXHAUSTIVE_H
#define EXHAUSTIVE_H

#include "OneMax.cpp"
#include <iostream>
#include <vector>
using namespace std;

class Exhaustive : OneMax
{
public:
    void RunALG(int, int, int);

private:
    // Input from Command-line Argument
    int Bit;
    int Run;
    int Iter;

    // calculate the evaluation
    int nfes; // now fitness
    int mnfes;// max fitness

    void Evaluation(vector<int>, int &);
    void Print();
    void Reset();
    auto Init()->vector<int>*;
};

void Exhaustive::RunALG(int Bit, int Run, int Iter)
{
    this->Bit = Bit;
    this->Run = Run;
    this->Iter = Iter;

    auto sol = Init();
    Evaluation (*sol, nfes);

}

void Exhaustive::Evaluation(vector<int> sol, int &nfes){
    vector<int> 
    for (int i=0; i<this->Iter; i++){

    }

}

void Exhaustive::Reset(){
    
}

auto Exhaustive::Init()->vector<int>*{
    vector<int> sol(this->Bit);
    for (int i=0; i<this->Bit; i++){
        sol[i] = rand()%2;
    }
    return &sol;
}

void Exhaustive::Print(){
    
}

#endif