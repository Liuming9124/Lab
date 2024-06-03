#ifndef PSOACK_H
#define PSOACK_H


#include "../problem/Ackley.cpp"
#include <queue>
#include <vector>


class Psoack: Ackley {
public:
    void RunALG( int, int, int, int, int);

private:
    int _Pop;
    int _Run;
    int _Iter;
    int _Bounder;

    typedef struct Particle{
        vector<float> _position, _velocity;
        vector<float> _pbest;
        float _fitness;
    } _Particle;

    _Particle _Gbest;
    vector<_Particle> _Swarm;


    void Init();
    void Evaluation();
    void Reset();

    float Frand();
    void CheckBorder(_Particle);

};

void Psoack::RunALG(int Pop, int Run, int Iter, int Dim, int Bounder){

    this->_Pop = Pop;
    this->_Run = Run;
    this->_Iter = Iter;
    this->_Bounder = Bounder;
    setArgs(20, 0.2, 2*M_PI, Dim);

    while (this->_Run--){
        cout << "-------------------Run" << Run - this->_Run << "---------------------" << endl;
        Init();
        Evaluation();
        Reset();
    }
}

void Psoack::Init(){
    _Swarm.resize(this->_Pop);
    int dim = getDim();
    for (int i=0; i<this->_Pop; i++){
        _Swarm[i]._position.resize(dim);
        _Swarm[i]._velocity.resize(dim);
        _Swarm[i]._pbest.resize(dim);
        for (int j=0; j<dim; j++){
            _Swarm[i]._position[j] = (rand() % _Bounder*2 - _Bounder);
            _Swarm[i]._velocity[j] = (rand() % _Bounder*2 - _Bounder);
            _Swarm[i]._pbest[j] = _Swarm[i]._position[j];
        }
        _Swarm[i]._fitness = AckleyProblem(_Swarm[i]._position);
        (i==0) ? _Gbest = _Swarm[i] : _Gbest = (_Gbest._fitness < _Swarm[i]._fitness) ? _Gbest : _Swarm[i];
    }
}

void Psoack::Evaluation(){
    for (int iter=0; iter<this->_Iter; iter++){
        for (int i=0; i<this->_Pop; i++){
            // TODO figure out how to set alpha1 and alpha2 and omega
            float alpha1 = 2;
            float alpha2 = 2;
            float omega = 0.5;
            for (int j=0; j<getDim(); j++){
                _Swarm[i]._velocity[j] = omega * _Swarm[i]._velocity[j] + alpha1 * Frand() * (_Swarm[i]._pbest[j] - _Swarm[i]._position[j]) + alpha2 * Frand() * (_Gbest._position[j] - _Swarm[i]._position[j]);
                _Swarm[i]._position[j] = _Swarm[i]._position[j] + _Swarm[i]._velocity[j];
                if(true){
                    // check border _Swarm[i]._position[j]
                    CheckBorder(_Swarm[i]);
                }
            }
            _Swarm[i]._fitness = AckleyProblem(_Swarm[i]._position);
            
            if (_Swarm[i]._fitness < AckleyProblem(_Swarm[i]._pbest)){
                _Swarm[i]._pbest = _Swarm[i]._position;
            }
            if (_Swarm[i]._fitness < AckleyProblem(_Gbest._pbest)){
                _Gbest = _Swarm[i];
            }
        }
        cout << "Iter: " << iter << " Best fitness: " << _Gbest._fitness << endl;
    }
    cout << "Best fitness: " << _Gbest._fitness << endl;
}


void Psoack::Reset(){
    _Swarm.clear();
    _Gbest._position.clear();
    _Gbest._velocity.clear();
    _Gbest._pbest.clear();
    _Gbest._fitness = 0;
}

float Psoack::Frand(){
    int r = rand()%1001;
    return r / 1000.0;
}

void Psoack::CheckBorder(_Particle check){
    for (int i = 0; i<getDim(); i++){
        if (!(check._position[i]<=this->_Bounder&& check._position[i]>=this->_Bounder)){
            check._position[i] = (rand() % _Bounder*2 - _Bounder);
        }
    }
}

#endif