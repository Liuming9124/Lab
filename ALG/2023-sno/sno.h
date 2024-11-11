#ifndef SNO_H
#define SNO_H
#include "../AlgPrint.h"
#include "../Tool.h"
#include "./cec2021.cpp"
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class SNO
{
public:
    typedef struct point
    {
        int _index;
        double _fitness;
        double _inCR, _inF;
        vector<double> _position;
    } T_Point;

    typedef struct net
    {
        int net_index;
        vector<int> point_index;
        double _Ia, _Ib;
        double _EVbest;
        double _EV;
    } T_Net;

public:
    void RunALG(int, int, int, int, int);

private:
    //env
    int num_Run;
    int num_Fess;
    int num_Dim;
    int num_Evals;
    int num_Netlen;

    // var
    int num_Point;
    int num_Region;

    double best_value;
    int eval_count;

    // tool
    AlgPrint show;
    Tool tool;
    Problem problem;

    // method
    void Init();
    void Evaluation();

    void expectedValue();
    void regionSearch();
    void pointSearch();
    void spaceNetAdjustment();
    void populationAdjustment();

    // DS
    void Init_T_point(vector<T_Point> &points, int size, int dim, bool posInitRand);
    void Init_T_net(vector<T_Net> &nets, int size, int num_region);

    static bool NetCompareFitness(const T_Net &a, const T_Net &b);
    int NetSelectTopPBest(vector<T_Net> xx, double p);
    static bool PointCompareFitness(const T_Point &a, const T_Point &b);

};

void SNO::RunALG(int Run, int Func, int Evals, int Dim, int NetLength)
{
    num_Run = Run;
    num_Fess = Evals;
    num_Dim = Dim;
    num_Netlen = NetLength;
    num_Region = (num_Netlen-1)*(num_Netlen-1);
    num_Point = num_Netlen*num_Netlen;

    show = AlgPrint(num_Run, "./result", "sno");
    show.NewShowDataDouble(num_Fess);

    problem.setStrategy(Func);

    while (num_Run--)
    {
        cout << "-------------------Run" << Run - num_Run << "---------------------" << endl;
        Init();
        Evaluation();
    }
    show.PrintToFileDouble("./result/result" + to_string(Func) + "_DIM" + to_string(num_Dim) + "_NetLen" + to_string(num_Netlen) + "_Fess" + to_string(num_Fess) + ".txt", num_Fess);
    cout << "end" << endl;

}

void SNO::Init()
{
    show.init();
    eval_count = 0;
    best_value = 0;
}

void SNO::Evaluation()
{
    while (eval_count < num_Fess)
    {
        expectedValue();
        regionSearch();
        spaceNetAdjustment();
        pointSearch();
        spaceNetAdjustment();
        populationAdjustment();
        // updateBestSoFar()
        // s=u, x=v, p=q
        
    }
}

# endif