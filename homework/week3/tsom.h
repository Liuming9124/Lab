#ifndef TSOM_H
#define TSOM_H

#include "../problem/OneMax.cpp"

class Tsom: OneMax {
public:
    void RunALG(int, int, int, double, double);

private:
    // Input from Command-line Argument
    int _Bit;
    int _Run;
    int _Iter;
    int _Tweaks;
    int _Tblen;

    int _Nfes;
    int _Mnfes;

    int _Iter_len = 0;

    void Init();
    void Evaluation();
    void Reset();

};

void Tsom::RunALG (int Bit, int Run, int Iter, double Tweaks, double Tblen){
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


void Tsom::Init(){

}
void Tsom::Evaluation(){

}
void Tsom::Reset(){

}

#endif