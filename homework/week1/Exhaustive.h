#ifndef EXHAUSTIVE_H
#define EXHAUSTIVE_H

#include "OneMax.cpp"
#include <iostream>
#include <vector>
using namespace std;

class Exhaustive : OneMax
{
public:
    void RunALG(int, int, int, double);

private:
    // Input from Command-line Argument
    int _Bit;
    int _Run;
    int _Iter;
    double _rate;

    // calculate the evaluation
    int nfes; // number fitness
    int mnfes;// mean number fitness

    vector<int> Init();
    void Evaluation(vector<int>);
    void Reset();

    void Adder(vector<int>*, int);
    void Print(vector<int>);
};

void Exhaustive::RunALG(int Bit, int Run, int Iter, double rate)
{
    this->_Bit = Bit;
    this->_Run = Run;
    this->_Iter = Iter;
    // this->rate = rate;
    // this->nfes = this->mnfes = 0;

    vector<int> sol = Init();
    Evaluation(sol);
    Reset();
}

void Exhaustive::Evaluation(vector<int> sol){
    vector<int> best = sol;
    vector<int> candidate = sol;
    for (int i=0; i<this->_Iter; i++){
        Adder(&candidate, 1);
        if (OneMaxProblem(candidate, this->_Bit) > OneMaxProblem(best, this->_Bit)){
            best = candidate;
            cout << "Better : ";
            Print(best);
        }
    }
    cout << "Best   : ";
    Print(best);
}

void Exhaustive::Reset(){
    this->nfes = 0;   
}

vector<int> Exhaustive::Init(){
    vector<int> sol(this->_Bit);
    for (int i=0; i<this->_Bit; i++){
        sol[i] = rand()%2;
    }
    return sol;
}

void Exhaustive::Print(vector<int> show){
    for (int x : show){
        cout << x;
    }
    cout << endl;
}

void Exhaustive::Adder(vector<int> *x, int y){
    int carry = y;
    int i = 0;
    while(1){
        int temp = (*x)[i]+ carry;
        (*x)[i] = temp%2;
        carry = temp/2;
        if (carry==1 && i==(*x).size()-1){
            (*x).push_back(1);
            break;
        }
        else if (carry==0){
            break;
        }
        i++;
    }
}

#endif