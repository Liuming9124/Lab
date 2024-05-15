#ifndef SA_H
#define SA_H

#include "Deception.cpp"
#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;
using std::setw;

class Sa : Deception
{
public:
    void RunALG(int, int, int, double, double);

private:
    // Input from Command-line Argument
    int _Bit;
    int _Run;
    int _Iter;
    double _Temp;
    double _Rate;

    int nfes;
    int mnfes;

    int _Iter_len = 0;
    vector<int> _end_value;

    vector<int> Init();
    void Evaluation(vector<int>);
    void Reset();

    void transaction(vector<int>*);
    void Print(int iter, vector<int>);
};

void Sa::RunALG(int Bit, int Run, int Iter, double Temp, double Rate){
    this->_Bit = Bit;
    this->_Run = Run;
    this->_Iter = Iter;
    this->_Temp = Temp;
    this->_Rate = Rate;
    this->nfes = this->mnfes = 0;

    while (this->_Run--){
        cout << "-------------------Run" << Run - this->_Run << "---------------------" << endl;
        vector<int> sol = Init();
        Evaluation(sol);
        Reset();
        cout << "test" << endl;
    }
    cout << "Average NFEs : " << this->mnfes/Run << endl;
}

void Sa::Evaluation(vector<int> sol){
    vector<int> best = sol;
    vector<int> candidate = sol;
    bool best_flag = false;
    for (int i=0; i<this->_Iter; i++){
        this->nfes++;
        transaction(&candidate);
        if ( DeceptionProblemCompare(candidate, best, this->_Bit) ){
            best = candidate;
            Print(i, best);
            if (this->_end_value == best){
                cout << "Best Solution Found before " << this->_Iter << endl;
                best_flag = true;
                break;
            }
        }
        else{
            double p = (rand()%10)/10.0;
            if (p > this->_Temp){
                best = candidate;
                Print(i, best);
            }
        }
        this->_Temp *= this->_Rate;
    }
    if (!best_flag){
        Print(this->_Iter, best);
    }
    
}

void Sa::Reset(){
    this->mnfes += this->nfes;
    cout << this->nfes << endl;
    this->nfes = 0;
    this->_Iter_len = 0;
}

vector<int> Sa::Init(){
    vector<int> sol(this->_Bit);
    for (int i=0; i<this->_Bit; i++){
        sol[i] = rand()%2;
    }
    int count = this->_Iter;
    do {
        this->_Iter_len++;
    } while (count/=10);
    this->_end_value = GetEndValue(this->_Bit);
    return sol;
}


void Sa::transaction(vector<int>* sol){
    int index = rand() % this->_Bit;
    (*sol)[index] = !(*sol)[index];
}

void Sa::Print(int iter, vector<int> show){
    cout << "Iter " << std::setw(this->_Iter_len) << iter << " : ";
    for (int x : show){
        cout << x;
    }
    cout << ", Value : ";
    for (int x: DeceptionProblem(show, this->_Bit)){
        cout << x;
    }
    cout << endl;

    // TODO: Write append without clear
    std::string filename = "../result/Sa/Sa_" + std::to_string(this->_Run) + ".txt";
    std::ofstream file(filename, std::ios_base::app);

    if (file.is_open()) {
        file << "Iter " << std::setw(10) << iter << " : ";
        for (int x : show) {
            file << x;
        }
        file << ", Value : ";
        for (int x: DeceptionProblem(show, this->_Bit)){
            file << x;
        }
        file << std::endl;
    }
    else {
        std::cerr << "Unable to open file!\n";
    }
}

#endif