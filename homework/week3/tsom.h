#ifndef TSOM_H
#define TSOM_H

#include "../problem/OneMax.cpp"

class Tsom: OneMax {
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

    std::vector<bool> _Sol;
    std::vector<bool> _Best;
    std::queue<std::vector<bool>> _Tblist;


    void Init();
    void Evaluation();
    void Reset();

    bool Inqueue(const std::vector<bool>&);
    std::vector<bool> TweakCp();

};

void Tsom::RunALG (int Bit, int Run, int Iter, int Tweaks, int Tblen){
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


void Tsom::Evaluation(){
    for (int iter=0; iter<this->_Iter && (OneMaxProblem(this->_Best, this->_Bit)!=this->_Bit); iter++){
        if (this->_Tblist.size() > this->_Tblen){
            this->_Tblist.pop();
        }
        std::vector<bool> R = TweakCp();
        this->_Nfes++;
        for (int i=0; i<this->_Tweaks-1; i++){
            this->_Nfes++;
            std::vector<bool> W = TweakCp();

            if ( !Inqueue(W) && ( OneMaxCompare(W, R, this->_Bit) || Inqueue(R)) ){
                R = W;
            }
        }
        if (!Inqueue(R)){
            this->_Sol = R;
            this->_Tblist.push(R);
        }
        if (OneMaxCompare(this->_Sol, this->_Best, this->_Bit)){
            this->_Best = this->_Sol;
        }
        Print(iter, this->_Best, 10, this->_Bit, this->_Run, "onemax", "Ts");
    }
}

void Tsom::Init(){
    this->_Best.resize(this->_Bit);
    this->_Sol.resize(this->_Bit);
    for (int i=0; i<this->_Bit; i++){
        this->_Best[i] = rand()%2;
    }
    this->_Sol = this->_Best;
    this->_Tblist.push(this->_Best);
}

void Tsom::Reset(){
    this->_Mnfes += this->_Nfes;
    this->_Nfes = 0;
    this->_Sol.clear();
    this->_Best.clear();
    this->_Tblist = std::queue<std::vector<bool>>();
}

std::vector<bool> Tsom::TweakCp(){
    std::vector<bool> sol(this->_Bit);
    sol = this->_Sol;
    Transaction( &sol, this->_Bit);
    return sol;
}

bool Tsom::Inqueue(const std::vector<bool> &check){
    std::queue<std::vector<bool>> tempQueue = _Tblist;
    while (!tempQueue.empty()) {
        if (tempQueue.front() == check) {
            return true;
        }
        tempQueue.pop();  // Remove the front element
    }
    return false;
}

#endif