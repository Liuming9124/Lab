#ifndef PSOACK_H
#define PSOACK_H


#include "../problem/Ackley.cpp"
#include <queue>


class Psoack: Ackley {
public:
    void RunALG( int, int, int);

private:
    int _Bit;
    int _Run;
    int _Iter;


    void Init();
    void Evaluation();
    void Reset();

};

void Psoack::RunALG(int Bit, int Run, int Iter){

    this->_Bit = Bit;
    this->_Run = Run;
    this->_Iter = Iter;
    
    while (this->_Run--){
        cout << "-------------------Run" << Run - this->_Run << "---------------------" << endl;
        Init();
        Evaluation();
        Reset();
    }
}

void Psoack::Init(){
}

void Psoack::Evaluation(){
}

void Psoack::Reset(){
}


#endif