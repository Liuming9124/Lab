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
    virtual double execute(const vector<int>& xx, int dim) const = 0;

};


class FuncOnemax : public Strategy {
public:
    FuncOnemax() {}
    double execute(const vector<int>& xx, int dim) const override {
        double num=0;
        for (int i=0; i<dim; i++){
            num += xx[i];
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
        case 0:
            strategy = make_unique<FuncOnemax>();
            break;
        default:
            cout << "Error: No such Function" << endl;
            break;    
        }
    }

    double executeStrategy(const vector<int>& xx, int dim) const { // TODO add evaulation times
        return strategy->execute(xx, dim);
    }

private:
    unique_ptr<Strategy> strategy;
};