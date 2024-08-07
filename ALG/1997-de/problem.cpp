#include <vector>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <string>
#include <memory>
#include "../Tool.h"
using namespace std;

class Strategy {
public:
    virtual double execute(const vector<double>& xx, int dim) const = 0;

    virtual void setBounds(double min, double max) {
        _BounderMin = min;
        _BounderMax = max;
    }
    virtual double getBounderMin() const {
        return _BounderMin;
    }
    virtual double getBounderMax() const {
        return _BounderMax;
    }

protected:
    double _BounderMin;
    double _BounderMax;
};

class FuncAckley : public Strategy {
    
public:
    FuncAckley() {
        _BounderMin = -32;
        _BounderMax = 32;
    }

    double execute (const vector<double>& xx, int _Dim) const override {
        double A = 20;
        double B = 0.2;
        double C = 2*M_PI;
        double sum1, sum2;
        for (int i=1; i<=_Dim; i++){
            sum1 += pow(xx[i], 2);
            sum2 += cos(C*xx[i]);
        }
        double term1 = (-A)*exp(-B*sqrt( sum1/_Dim));
        double term2 = -exp( sum2/_Dim );

        return term1 + term2 + A + exp(1);
    }
};

class Func1 : public Strategy {
public:
    Func1() {
        _BounderMin = -100.0;
        _BounderMax = 100.0;
    }
    double execute(const vector<double>& xx, int dim) const override {
        double num=0;
        for (int i=0; i<dim; i++){
            num += xx[i]*xx[i];
        }
        return num;
    }
};

class Func2 : public Strategy {
public:
    Func2() {
        _BounderMin = -10.0;
        _BounderMax = 10.0;
    }
    double execute(const vector<double>& xx, int dim) const override {
        double num=0;
        for (int i=0; i<dim; i++){
            if (i==0) {
                if (xx[i]>=0){
                    num += xx[i];
                }
                else{
                    num -= xx[i];
                }
                continue;
            }
            if (xx[i]>=0){
                num *= xx[i];
            }
            else{
                num *= -xx[i];
            }
        }
        for (int i=0; i<dim; i++){
            if (xx[i]>=0){
                num += xx[i];
            }
            else{
                num -= xx[i];
            }
        } 
        return num;
    }
};



class Func3 : public Strategy {
public:
    Func3() {
        _BounderMin = -100.0;
        _BounderMax = 100.0;
    }
    double execute(const vector<double>& xx, int dim) const override {
        double num=0;
        for (int i=0; i<dim; i++){
            double temp = 0;
            for (int j=0; j<=i; j++){
                temp += xx[j];
            }
            num += temp*temp;
        }
        return num;
    }
};

class Func4 : public Strategy {
public:
    Func4() {
        _BounderMin = -100.0;
        _BounderMax = 100.0;
    }
    double execute(const vector<double>& xx, int dim) const override {
        double num = abs(xx[0]);
        for (int i=1; i<dim; i++){
            if (abs(xx[i])>num){
                num = abs(xx[i]);
            }
        }
        return num;
    }
};

class Func5 : public Strategy {
public:
    Func5() {
        _BounderMin = -30.0;
        _BounderMax = 30.0;
    }
    double execute(const vector<double>& xx, int dim) const override {
        double num = 0;
        for (int i=0; i<dim-1; i++){
            double t1, t2;
            t1=t2=0;
            t1 = xx[i+1]-xx[i]*xx[i];
            t1 = 100*(t1*t1);
            t2 = (xx[i]-1)*(xx[i]-1);
            num += t1+t2;
        }
        return num;
    }
};

class Func6 : public Strategy {
public:
    Func6() {
        _BounderMin = -100.0;
        _BounderMax = 100.0;
    }
    double execute(const vector<double>& xx, int dim) const override {
        double num = 0;
        for(int i=0; i<dim; i++){
            num += floor(xx[i]+0.5) * floor(xx[i]+0.5);
        }
        return num;
    }
};

class Func7 : public Strategy {
public:
    Func7() {
        _BounderMin = -1.28;
        _BounderMax = 1.28;
    }
    double execute(const vector<double>& xx, int dim) const override {
        Tool tool;
        double num = 0;
        for(int i=0; i<dim; i++){
            double tmp=0;
            do {
                tmp = tool.rand_double(0,1);
            } while (tmp==1);
            num += i*(xx[i]*xx[i]*xx[i]*xx[i]) + tmp;
        }
        return num;
    }
};

class Func8 : public Strategy {
public:
    Func8() {
        _BounderMin = -500.0;
        _BounderMax = 500.0;
    }
    double execute(const vector<double>& xx, int dim) const override {
        double num = 0;
        for (int i=0; i<dim; i++){
            num += -xx[i]*sin(sqrt(abs(xx[i])));
        }
        num += dim*418.98288727243369;
        return num;
    }
};

class Func9 : public Strategy {
public:
    Func9() {
        _BounderMin = -5.12;
        _BounderMax = 5.12;
    }
    double execute(const vector<double>& xx, int dim) const override {
        double num = 0;
        for (int i=0; i<dim; i++){
            num += xx[i]*xx[i]-10*cos(2*M_PI*xx[i])+10;
        }
        return num;
    }
};

class Func10 : public Strategy {
public:
    Func10() {
        _BounderMin = -32.0;
        _BounderMax = 32.0;
    }
    double execute(const vector<double>& xx, int dim) const override {
        double num = 0;
        double t1, t2;
        t1 = t2 = 0;
        for (int i=0; i<dim; i++){
            t1 += xx[i]*xx[i];
            t2 += cos(2*M_PI*xx[i]);
        }
        num += -20*exp(-0.2*sqrt(t1/dim))-exp(t2/dim)+20+exp(1);
        return num;
    }
};

class Func11 : public Strategy {
public:
    Func11() {
        _BounderMin = -600.0;
        _BounderMax = 600.0;
    }
    double execute(const vector<double>& xx, int dim) const override {
        double num = 0;
        double t1, t2;
        t1 = t2 = 0;
        for (int i=0; i<dim; i++){
            t1 += xx[i]*xx[i];
            if (i==0){
                t2 += cos(xx[i]/sqrt(i+1));
            }
            else{
                t2 *= cos(xx[i]/sqrt(i+1));
            }
        }
        num = t1/4000 - t2 + 1;
        return num;
    }
};

class Func12 : public Strategy {
public:
    double execute(const vector<double>& xx, int dim) const override {
        double num = 0;
        
        return num;
    }
};


class Problem
{
public:
    void setStrategy(int func) {
        switch (func)
        {
        case 0:
            strategy = make_unique<FuncAckley>();
            break;
        case 1:
            strategy = make_unique<Func1>();
            break;
        case 2:
            strategy = make_unique<Func2>();
            break;
        case 3:
            strategy = make_unique<Func3>();
            break;
        case 4:
            strategy = make_unique<Func4>();
            break;
        case 5:
            strategy = make_unique<Func5>();
            break;
        case 6:
            strategy = make_unique<Func6>();
            break;
        case 7:
            strategy = make_unique<Func7>();
            break;
        case 8:
            strategy = make_unique<Func8>();
            break;
        case 9:
            strategy = make_unique<Func9>();
            break;
        case 10:
            strategy = make_unique<Func10>();
            break;
        case 11:
            strategy = make_unique<Func11>();
            break;
        default:
            cout << "Error: No such Function" << endl;
            break;    
        }
    }

    double executeStrategy(const vector<double>& xx, int dim) const {
        return strategy->execute(xx, dim);
    }


    void setBounds(double min, double max) {
        if (strategy) {
            strategy->setBounds(min, max);
        }
    }

    double getBounderMin() const {
        return strategy ? strategy->getBounderMin() : 0.0;
    }

    double getBounderMax() const {
        return strategy ? strategy->getBounderMax() : 0.0;
    }

private:
    unique_ptr<Strategy> strategy;

};