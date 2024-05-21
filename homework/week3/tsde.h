#ifndef TSDE_H
#define TSDE_H

#include "../problem/Deception.cpp"
#include <queue>


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
    this->_Bit = Bit;
    this->_Run = Run;
    this->_Iter = Iter;
    this->_Tweaks = Tweaks;
    this->_Tblen = Tblen;
    this->_Nfes = this->_Mnfes = 0;

    while (this->_Run--){
        cout << "-------------------Run" << Run - this->_Run << "---------------------" << endl;
        Init();
        Evaluation();
        Reset();
    }
    cout << "Average NFEs : " << this->_Mnfes/Run << endl;
}


void Tsde::Evaluation(){
    for (int iter=0; iter<this->_Iter && (this->_Best!=this->_End_value); iter++){
        if (this->_Tblist.size() > this->_Tblen){
            this->_Tblist.pop();
        }
        std::vector<int> R = TweakCp();
        this->_Nfes++;
        for (int i=0; i<this->_Tweaks-1; i++){
            this->_Nfes++;
            std::vector<int> W = TweakCp();

            if ( !Inqueue(W) && ( DeceptionProblemCompare(W, R) || Inqueue(R)) ){
                R = W;
            }
        }
        if (!Inqueue(R)){
            this->_Sol = R;
            this->_Tblist.push(R);
        }
        if (DeceptionProblemCompare(this->_Sol, this->_Best)){
            this->_Best = this->_Sol;
        }
        Print(iter, this->_Best, 10, this->_Bit, this->_Run, "Deception", "Ts");
    }
}

void Tsde::Init(){
    this->_Best.resize(this->_Bit);
    this->_Sol.resize(this->_Bit);
    for (int i=0; i<this->_Bit; i++){
        this->_Best[i] = rand()%2;
    }
    this->_Sol = this->_Best;
    this->_Tblist.push(this->_Best);
    this->_End_value = GetEndValue(this->_Bit);
}

void Tsde::Reset(){
    this->_Mnfes += this->_Nfes;
    this->_Nfes = 0;
    this->_Sol.clear();
    this->_Best.clear();
    this->_Tblist = std::queue<std::vector<int>>();
    this->_End_value.clear();
}

std::vector<int> Tsde::TweakCp(){
    std::vector<int> sol(this->_Bit);
    sol = this->_Sol;
    Transaction( &sol, this->_Bit);
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