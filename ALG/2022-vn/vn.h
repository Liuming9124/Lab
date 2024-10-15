#ifndef VN_H
#define VN_H

#include "../AlgPrint.h"
#include "../Tool.h"
#include "./cec2021.cpp"
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
        double _EVbest;
        double _EV;
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

    double best_value;

    // evaluation count
    int eval_count; 
        
    // tool
    AlgPrint show;
    Tool tool;
    Problem problem;

    void Init();
    void Evaluation();

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
    }
    show.PrintToFileDouble("./result/result" + to_string(Func) + "_DIM" + to_string(num_Dim) + "_NetLen" + to_string(num_Netlen) + "_Fess" + to_string(num_Fess) + ".txt", num_Fess);
    cout << "end" << endl;
}

void VN::Init()
{
    show.init();
    eval_count = 0;
    best_value = 0;

    // history memory size
    history_index = 0;
    HistoryTable.assign(num_History, {0.3, 0.3});
    SCR.resize(num_Netlen * num_Netlen);
    SF.resize(num_Netlen * num_Netlen);

    // init point
    len_x = num_Netlen * num_Netlen;
    X.resize(len_x);
    X_previous.resize(len_x);
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
    Net.resize(len_net);
    for (int i=0; i<num_Netlen-1; i++){
        for (int j=0; j<num_Netlen-1; j++){
            Net[i*(num_Netlen-1)+j].net_index = i*(num_Netlen-1)+j;
            Net[i*(num_Netlen-1)+j].point_index.assign(4, 0);
            Net[i*(num_Netlen-1)+j].point_index[0] = i*num_Netlen + j;
            Net[i*(num_Netlen-1)+j].point_index[1] = i*num_Netlen + j + 1;
            Net[i*(num_Netlen-1)+j].point_index[2] = (i+1)*num_Netlen + j;
            Net[i*(num_Netlen-1)+j].point_index[3] = (i+1)*num_Netlen + j + 1;
            Net[i*(num_Netlen-1)+j]._Ia = Net[i*(num_Netlen-1)+j]._Ib = 1;
            Net[i*(num_Netlen-1)+j]._EV = 0;
            // calculate Ebest
            Net[i*(num_Netlen-1)+j]._EVbest  = X[Net[i*(num_Netlen-1)+j].point_index[0]]._fitness;
            for (int k=1; k<4; k++){
                if (Net[i*(num_Netlen-1)+j]._EVbest > X[Net[i*(num_Netlen-1)+j].point_index[k]]._fitness){
                    Net[i*(num_Netlen-1)+j]._EVbest = X[Net[i*(num_Netlen-1)+j].point_index[k]]._fitness;
                }
            }
        }
    }
    // sort point and save to tmp variable and set best value
    vector<T_Point> tmp = X;
    sort(tmp.begin(), tmp.end(), PointCompareFitness);
    best_value = tmp[0]._fitness;
    show.SetDataDouble(num_Run, tmp[0]._fitness, eval_count);
}

void VN::Evaluation()
{
    while (eval_count < num_Fess)
    {
        if (eval_count > num_Fess)
            break;
        // expected value
        expected_value();
        // net update
        net_update();
        // show data
        vector<T_Point> tmp = X;
        sort(tmp.begin(), tmp.end(), PointCompareFitness);
        if (best_value > tmp[0]._fitness)
            best_value = tmp[0]._fitness;
        // if (eval_count > num_Fess){
        //     show.SetDataDouble(num_Run, best_value, num_Fess);
        //     // break;
        // }
        if (eval_count >= num_Fess)
            show.SetDataDouble(num_Run, best_value, num_Fess);
        else
            show.SetDataDouble(num_Run, best_value, eval_count);
        // cout << "eval_count: " << eval_count << " best: " << best_value << endl;
    }
}

void VN::expected_value()
{
    // compute each net's expected value
    vector<double> visit_ratio(len_net, 0);
    vector<double> increase_ratio(len_net, 0);
    vector<double> EV(len_net, 0);

    for (int i = 0; i < len_net; i++)
    {
        visit_ratio[i] = Net[i]._Ib / Net[i]._Ia;
        for (int j = 0; j < 4; j++)
        {
            increase_ratio[i] += X[Net[i].point_index[j]]._fitness - X_previous[Net[i].point_index[j]]._fitness;
        }
        increase_ratio[i] /= 4;
        EV[i] = Net[i]._EVbest;
    }
    // min-max normalization of three items
    double max_visit_ratio = *max_element(visit_ratio.begin(), visit_ratio.end());
    double min_visit_ratio = *min_element(visit_ratio.begin(), visit_ratio.end());
    double max_increase_ratio = *max_element(increase_ratio.begin(), increase_ratio.end());
    double min_increase_ratio = *min_element(increase_ratio.begin(), increase_ratio.end());
    double max_EV = *max_element(EV.begin(), EV.end());
    double min_EV = *min_element(EV.begin(), EV.end());
    for (int i = 0; i < len_net; i++)
    {
        if (max_visit_ratio - min_visit_ratio == 0)
            visit_ratio[i] = 0;
        else
            visit_ratio[i] = (visit_ratio[i] - min_visit_ratio) / (max_visit_ratio - min_visit_ratio);
        if (max_increase_ratio - min_increase_ratio == 0)
            increase_ratio[i] = 0;
        else
            increase_ratio[i] = (increase_ratio[i] - min_increase_ratio) / (max_increase_ratio - min_increase_ratio);
        if (max_EV - min_EV == 0)
            EV[i] = 0;
        else
            EV[i] = 1 - (EV[i] - min_EV) / (max_EV - min_EV); // due to minimize problem
    }
    // compute expected value
    double omega = 1.5 - (0.5*eval_count)/num_Fess;
    for (int i = 0; i < len_net; i++)
    {
        Net[i]._EV = visit_ratio[i] + increase_ratio[i] + EV[i] * omega;
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
    double pbest = 0.4 - ( (0.2/eval_count) / num_Fess );
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
            Net[i]._Ib++;
    }

    // select pbest point in pbest net
    int point_pbest = PointSelectTopPBest(Net[net_pbest].point_index, pbest);

    // update every point in net
    vector<T_Point> U, V;
    U.resize(len_x);
    V.resize(len_x);
    // init U & V
    for (int i=0; i<len_x; i++){
        U[i]._index = V[i]._index =  i;
        U[i]._position.resize(num_Dim);
        V[i]._position.resize(num_Dim);
        for (int j=0; j<num_Dim; j++)
            U[i]._position[j] = V[i]._position[j] = 0;
        U[i]._fitness = V[i]._fitness = 0;
        U[i]._inCR = U[i]._inF = V[i]._inCR = V[i]._inF = 0;
    }

    for (int i=0; i<len_x; i++){
        // randomly select cr&f from history table
        int index = tool.rand_int(0, num_History - 1);
        // CR
        X[i]._inCR = tool.rand_normal(HistoryTable[index]._MCR, 0.1);
        if (X[i]._inCR > 1)
            X[i]._inCR = 1;
        else if (X[i]._inCR < 0)
            X[i]._inCR = 0;
        // F
        X[i]._inF = 0;
        do
        {
            X[i]._inF = tool.rand_cauchy(HistoryTable[index]._MF, 0.1);
            if (X[i]._inF >= 1)
                X[i]._inF = 1;
        } while (X[i]._inF <= 0);

        // random select r1, r2
        int r1 = tool.rand_int(0, len_x - 1);
        int r2 = tool.rand_int(0, len_x - 1);
        while (r1 == i)
            r1 = tool.rand_int(0, len_x - 1);
        while (r2 == i || r2 == r1)
            r2 = tool.rand_int(0, len_x - 1);
        for (int j=0; j<num_Dim; j++){
            // mutation
            V[i]._position[j] = X[i]._position[j] 
                                + X[i]._inF * (X[point_pbest]._position[j] - X[i]._position[j]) 
                                + X[i]._inF * (X[r1]._position[j] - X[r2]._position[j]);
            // check boundary
            if (V[i]._position[j] < problem.getBounderMin() || V[i]._position[j] > problem.getBounderMax())
                V[i]._position[j] = tool.rand_double(problem.getBounderMin(), problem.getBounderMax());

            // crossover
            int jrand = tool.rand_int(0, num_Dim - 1);
            if (jrand == j || tool.rand_double(0, 1) < X[i]._inCR)
                U[i]._position[j] = V[i]._position[j];
            else
                U[i]._position[j] = X[i]._position[j];
        }
        U[i]._fitness = problem.executeStrategy(U[i]._position, num_Dim);
        U[i]._inCR = X[i]._inCR;
        U[i]._inF = X[i]._inF;
        
        eval_count++;
        // update scr & sf
        if (X[i]._fitness > U[i]._fitness)
        {
            SCR.push_back(U[i]._inCR);
            SF.push_back(U[i]._inF);
            deltaF.push_back(X[i]._fitness - U[i]._fitness);
        }
        // update solution
        X_previous[i] = X[i];
        X[i] = U[i];
    }

    // Update HS
    if (SCR.size() != 0 && SF.size() != 0)
    {
        // prepare param
        double mCR, mF, WKdenominator, numerator, denominator;
        mCR = mF = WKdenominator = numerator = denominator = 0;

        for (int t = 0; t < SCR.size(); t++)
            WKdenominator += deltaF[t];

        // Update MCR
        for (int t = 0; t < SCR.size(); t++)
        {
            // mean weight Scr
            mCR += (deltaF[t] / WKdenominator) * SCR[t];
        }
        HistoryTable[history_index]._MCR = mCR;

        // Update MF
        for (int t = 0; t < SCR.size(); t++)
        {
            // Lehmer mean
            numerator += (deltaF[t] / WKdenominator) * SF[t] * SF[t];
            denominator += (deltaF[t] / WKdenominator) * SF[t];
        }
        mF = numerator / denominator;
        HistoryTable[history_index]._MF = mF;

        history_index++;
        if (history_index == num_History)
            history_index = 0;
    }
}


bool VN::NetCompareFitness(const T_Net &a, const T_Net &b)
{
    // return descending order
    return a._EV > b._EV;
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
    // return ascending order
    return a._fitness < b._fitness;
}

int VN::PointSelectTopPBest(vector<int> xx, double p)
{
    vector<T_Point> tmp;
    tmp.resize(4);
    for (int i = 0; i < 4; i++)
    {
        tmp[i] = X[xx[i]];
        tmp[i]._index = xx[i];
    }
    sort(tmp.begin(), tmp.end(), PointCompareFitness);
    int place;
    place = p * 3;
    place = tool.rand_int(0, place);
    return tmp[place]._index;
}

#endif