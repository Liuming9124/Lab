#include <vector>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <string>
#include <memory>
#include "../Tool.h"
using namespace std;
#pragma once

class Strategy {
public:
    virtual long double execute(const vector<long double>& xx, int dim) const = 0;
};

class Func1 : public Strategy {
public:
    long double execute(const vector<long double>& xx, int dim) const override {
        long double num=0;
        for (int i=0; i<dim; i++){
            num += xx[i]*xx[i];
        }
        return num;
    }
};

class Func2 : public Strategy {
public:
    long double execute(const vector<long double>& xx, int dim) const override {
        long double num=0;
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
    long double execute(const vector<long double>& xx, int dim) const override {
        long double num=0;
        for (int i=0; i<dim; i++){
            long double temp = 0;
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
    long double execute(const vector<long double>& xx, int dim) const override {
        long double num = abs(xx[0]);
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
    long double execute(const vector<long double>& xx, int dim) const override {
        long double num = 0;
        for (int i=0; i<dim-1; i++){
            long double t1, t2;
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
    long double execute(const vector<long double>& xx, int dim) const override {
        long double num = 0;
        for(int i=0; i<dim; i++){
            num += floor(xx[i]+0.5) * floor(xx[i]+0.5);
        }
        return num;
    }
};

class Func7 : public Strategy {
public:
    long double execute(const vector<long double>& xx, int dim) const override {
        Tool tool;
        long double num = 0;
        for(int i=0; i<dim; i++){
            double tmp=0;
            do {
                tmp = tool.rand_float(0,1);
            } while (tmp==1);
            num += i*(xx[i]*xx[i]*xx[i]*xx[i]) + tmp;
        }
        return num;
    }
};

class Func8 : public Strategy {
public:
    long double execute(const vector<long double>& xx, int dim) const override {
        long double num = 0;
        for (int i=0; i<dim; i++){
            num += -xx[i]*sin(sqrt(abs(xx[i])));
        }
        num += dim*418.98288727243369;
        return num;
    }
};

class Func9 : public Strategy {
public:
    long double execute(const vector<long double>& xx, int dim) const override {
        long double num = 0;
        for (int i=0; i<dim; i++){
            num += xx[i]*xx[i]-10*cos(2*M_PI*xx[i])+10;
        }
        return num;
    }
};

class Func10 : public Strategy {
public:
    long double execute(const vector<long double>& xx, int dim) const override {
        long double num = 0;
        long double t1, t2;
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
    long double execute(const vector<long double>& xx, int dim) const override {
        long double num = 0;
        long double t1, t2;
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
    long double execute(const vector<long double>& xx, int dim) const override {
        long double num = 0;
        
        return num;
    }
};


class Problem
{
public:
    void setStrategy(unique_ptr<Strategy> strat) {
        strategy = move(strat);
    }

    long double executeStrategy(const vector<long double>& xx, int dim) const {
        return strategy->execute(xx, dim);
    }

private:
    unique_ptr<Strategy> strategy;
};