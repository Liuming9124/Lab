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

class Func1 : public Strategy {
public:
    Func1() {
        _BounderMin =  1000.0;
        _BounderMax = -1000.0;
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
        _BounderMin =  1000.0;
        _BounderMax = -1000.0;
    }
    double execute(const vector<double>& xx, int dim) const override {
        double num=0;
        for (int i=0; i<dim; i++){
            num += (xx[i]-2) * (xx[i]-2);
        }
        return num;
    }
};

class Problem
{
public:
    void setStrategy(int func) {
        switch (func)
        {
        case 1:
            strategy = make_unique<Func1>();
            break;
        case 2:
            strategy = make_unique<Func2>();
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