#ifndef VN_H
#define VN_H

#include "../AlgPrint.h"
#include "../Tool.h"
#include "../CEC/test13.cpp"
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class VN
{
public:
    typedef struct history
    {
        double _MCR, _MF;
    } T_History;

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
        double _Ebest;
        double _E;
    } T_Net;

public:
    void RunALG(int, int, int, int, int, double);

private:
    // env
    int num_Run;
    int num_Fess;
    int num_Dim;
    int num_Evals;
    int num_Netlen;
    int num_History;

    // var
    int history_index;
    vector<T_History> HistoryTable;   // history table
    vector<T_Point> X, X_previous;    // point
    vector<T_Net> Net;    // net

    int len_x;
    int len_net;

    vector<double> SF, SCR;
    int index_success = 0;

    vector<double> best_sol;
    double best_value;

    // evaluation count
    int eval_count; 
        
    // tool
    AlgPrint show;
    Tool tool;
    Problem problem;

    void Init();
    void Evaluation();
    void Reset();

    void expected_value();
    void net_update();
 
    static bool NetCompareFitness(const T_Net &a, const T_Net &b);
    int NetSelectTopPBest(vector<T_Net> xx, double p);
    
    static bool PointCompareFitness(const T_Point &a, const T_Point &b);
    int PointSelectTopPBest(vector<int> xx, double p);
};

void VN::RunALG(int Run, int Func, int Evals, int Dim, int NetLength, double History)
{
    num_Run = Run;
    num_Fess = Evals;
    num_Dim = Dim;
    num_Netlen = NetLength;
    num_History = History * num_Dim;

    show = AlgPrint(num_Run, "./result", "vn");
    show.NewShowDataDouble(num_Fess);

    problem.setStrategy(Func);

    while (num_Run--)
    {
        cout << "-------------------Run" << Run - num_Run << "---------------------" << endl;
        Init();
        Evaluation();
        Reset();
    }
    show.PrintToFileDouble("./result/result" + to_string(Func) + "_DIM" + to_string(num_Dim) + "_NetLen" + to_string(num_Netlen) + "_Fess" + to_string(NetLength) + ".txt", Run);
    cout << "end" << endl;
}

void VN::Init()
{
    eval_count = 0;
    best_sol.assign(num_Dim, 0);
    best_value = 0;

    // history memory size
    history_index = 0;
    HistoryTable.assign(num_History, {0.3, 0.3});
    
    
    SCR.resize(num_Netlen * num_Netlen);
    SF.resize(num_Netlen * num_Netlen);

    // init point
    len_x = num_Netlen * num_Netlen;
    for (int i=0; i<len_x; i++){
        // init X
        X[i]._index = i;
        X[i]._position.assign(num_Dim, 0);
        for (int j = 0; j < num_Dim; j++)
        {
            X[i]._position[j] = tool.rand_double(problem.getBounderMin(), problem.getBounderMax());
        }
        X[i]._fitness = problem.executeStrategy(X[i]._position, num_Dim);
        X[i]._inCR = X[i]._inF = 0;
        eval_count++;
        // init X_previous
        X_previous[i]._index = i;
        X_previous[i]._position.assign(num_Dim, 0);
        X_previous[i]._fitness = 0;
        X_previous[i]._inCR = X_previous[i]._inF = 0;
    }
    // init net
    len_net = (num_Netlen-1)*(num_Netlen-1);
    for (int i=0; i<len_net; i++){
        Net[i].net_index = i;
        Net[i].point_index.assign(4, 0);
        for (int j=0; j<num_Netlen; j++){
            Net[i].point_index[0] = i + j*num_Netlen;
            Net[i].point_index[1] = i + j*num_Netlen + 1;
            Net[i].point_index[2] = i + j*num_Netlen + num_Netlen;
            Net[i].point_index[3] = i + j*num_Netlen + num_Netlen + 1; 
        }
        Net[i]._Ia = Net[i]._Ib = 1;
        Net[i]._E = 0;
        // calculate Ebest
        Net[i]._Ebest  = X[Net[i].point_index[0]]._fitness;
        for (int j=1; j<4; j++){
            if (Net[i]._Ebest > X[Net[i].point_index[j]]._fitness){
                Net[i]._Ebest = X[Net[i].point_index[j]]._fitness;
            }
        }
    }
}

void VN::Evaluation()
{
    while (eval_count < num_Fess)
    {
        // expected value
        expected_value();
        // net update
        net_update();
        // show data
        show.SetDataDouble(num_Run, best_value, eval_count);
    }
}

void VN::expected_value()
{
    // compute each net's expected value
    vector<double> visit_ratio(len_net, 0);
    vector<double> increase_ratio(len_net, 0);
    vector<double> E(len_net, 0);

    for (int i = 0; i < len_net; i++)
    {
        visit_ratio[i] = Net[i]._Ib / Net[i]._Ia;
        for (int j = 0; j < 4; j++)
        {
            increase_ratio[i] += X[Net[i].point_index[j]]._fitness - X_previous[Net[i].point_index[j]]._fitness;
        }
        increase_ratio[i] /= 4;
        E[i] = Net[i]._Ebest;
    }
    // min-max normalization of three items
    double max_visit_ratio = *max_element(visit_ratio.begin(), visit_ratio.end());
    double min_visit_ratio = *min_element(visit_ratio.begin(), visit_ratio.end());
    double max_increase_ratio = *max_element(increase_ratio.begin(), increase_ratio.end());
    double min_increase_ratio = *min_element(increase_ratio.begin(), increase_ratio.end());
    double max_E = *max_element(E.begin(), E.end());
    double min_E = *min_element(E.begin(), E.end());
    for (int i = 0; i < len_net; i++)
    {
        visit_ratio[i] = (visit_ratio[i] - min_visit_ratio) / (max_visit_ratio - min_visit_ratio);
        increase_ratio[i] = (increase_ratio[i] - min_increase_ratio) / (max_increase_ratio - min_increase_ratio);
        E[i] = (E[i] - min_E) / (max_E - min_E);
    }
    // compute expected value
    double omega = 1.5 - (0.5*eval_count)/num_Fess;
    for (int i = 0; i < len_net; i++)
    {
        Net[i]._E = visit_ratio[i] + increase_ratio[i] + E[i] * omega;
    }
}

void VN::net_update()
{
    // Evaluate data
    vector<double> deltaF; // to store fitness to calculate mean
    deltaF.clear();
    SCR.clear();
    SF.clear();

    // select pbest net
    int pbest = 0.4 - ( (0.2/eval_count) / num_Fess );
    int net_pbest = NetSelectTopPBest(Net, pbest);

    // update Ia, Ib
    for (int i = 0; i < len_net; i++)
    {
        if (i == net_pbest)
        {
            Net[i]._Ia++;
            Net[i]._Ib = 1;
        }
        else
        {
            Net[i]._Ib++;
        }
    }

    // select pbest point in pbest net
    int point_pbest = PointSelectTopPBest(Net[net_pbest].point_index, pbest);
    
    // update every point in net
    vector<T_Point> tmp;
    tmp.resize(len_x);
    for (int i=0; i<len_x; i++){
        // randomly select cr&f from history table
        int index = tool.rand_int(0, num_History - 1);
        // CR
        tmp[i]._inCR = tool.rand_normal(HistoryTable[index]._MCR, 0.1);
        if (tmp[i]._inCR > 1)
            tmp[i]._inCR = 1;
        else if (tmp[i]._inCR < 0)
            tmp[i]._inCR = 0;
        // F
        tmp[i]._inF = 0;
        do
        {
            tmp[i]._inF = tool.rand_cauchy(HistoryTable[index]._MF, 0.1);
            if (tmp[i]._inF >= 1)
                tmp[i]._inF = 1;
        } while (tmp[i]._inF <= 0);

        for (int j=0; j<num_Dim; j++){
            // random select r1, r2
            int r1 = tool.rand_int(0, len_x - 1);
            int r2 = tool.rand_int(0, len_x - 1);
            while (r1 == i)
                r1 = tool.rand_int(0, len_x - 1);
            while (r2 == i || r2 == r1)
                r2 = tool.rand_int(0, len_x - 1);
            // mutation
            tmp[i]._position[j] = X[i]._position[j] 
                                + tmp[i]._inF * (X[point_pbest]._position[j] - X[i]._position[j]) 
                                + tmp[i]._inF * (X[r1]._position[j] - X[r2]._position[j]);
            // check boundary
            if (tmp[i]._position[j] < problem.getBounderMin())
                tmp[i]._position[j] = tool.rand_double(problem.getBounderMin(), problem.getBounderMax());

            // crossover
            int jrand = tool.rand_int(0, num_Dim - 1);
            if (jrand == j || tool.rand_double(0, 1) < tmp[i]._inCR)
                tmp[i]._position[j] = X[i]._position[j];
        }
        tmp[i]._fitness = problem.executeStrategy(tmp[i]._position, num_Dim);
        // update scr & sf
        if (X[i]._fitness > tmp[i]._fitness)
        {
            SCR.push_back(tmp[i]._inCR);
            SF.push_back(tmp[i]._inF);
            // deltaF.push_back(tmp[i]._fitness - _U._fitness);
        }

    }

}

void VN::Reset()
{
}



bool VN::NetCompareFitness(const T_Net &a, const T_Net &b)
{
    return a._E < b._E;
}

int VN::NetSelectTopPBest(vector<T_Net> xx, double p)
{
    vector<T_Net> tmp = xx;
    sort(tmp.begin(), tmp.end(), NetCompareFitness);
    int place;
    place = p * len_net;
    place = tool.rand_int(0, place);
    return tmp[place].net_index;
}

bool VN::PointCompareFitness(const T_Point &a, const T_Point &b)
{
    return a._fitness < b._fitness;
}

int VN::PointSelectTopPBest(vector<int> xx, double p)
{
    vector<T_Point> tmp;
    for (int i = 0; i < 4; i++)
    {
        tmp.push_back(X[xx[i]]);
    }
    sort(tmp.begin(), tmp.end(), PointCompareFitness);
    int place;
    place = p * len_net;
    place = tool.rand_int(0, place);
    return tmp[place]._index;
}

#endif