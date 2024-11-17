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
        int _netIndex;
        double _fitness;
        double _inCR, _inF;
        vector<double> _position;
    } T_Point;

    typedef struct net
    {
        int net_index;
        vector<int> point_index;
        double _mC, _mF;
        double _Va, _Vb;
        double _EVbest;
        double _EV;
    } T_Net;

public:
    void RunALG(int, int, int, int, int, int, int, int, int, double, double, double, double, double, double);
private:
    //env
    int num_Run;
    int num_Fess;
    int num_Dim;
    int num_Eval;
    int num_Netlen;
    int num_Explorer;
    int num_MinerInit;
    int num_MinerFinal;
    int num_Adjustment;
    double num_rho;
    double num_mf;
    double num_mc;
    double num_alpha;
    double num_beta;
    double num_archive;

    // parameter of algorithm
    int num_Miner;
    vector<T_Point> s, x; // s: explorer, x: miner
    vector<T_Point> Points, Points_previous;
    vector<T_Net> Net;
    vector<double> SF, SCR, SRegion, deltaF;
    vector<T_Point> A;

    // var
    int eval_count;
    int num_Point;
    int num_Region;

    double best_value;

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
    void updateBestSoFar();

    // DS
    void Init_T_point(vector<T_Point> &points, int size, int dim, bool posInitRand);
    void Init_T_net(vector<T_Net> &nets, int size, int num_region);

    static bool NetCompareFitness(const T_Net &a, const T_Net &b);
    int NetSelectTopPBest(vector<T_Net> xx, double p);
    static bool PointCompareFitness(const T_Point &a, const T_Point &b);

};
// Run, Func, Evals, Dim, NetLength, Explorer, MinerInit, MinerFinal, AdjustmentMax, Rho, mf, mc, Alpha, Beta, Archive
void SNO::RunALG(int Run, int Func, int Eval, int Dim, int NetLength, int Explorer, int MinerInit, int MinerFinal, int Adjustment, double rho, double mf, double mc, double Alpha, double Beta, double Archive)
{
    num_Run = Run;
    num_Eval = Eval;
    num_Dim = Dim;
    num_Netlen = NetLength;
    num_Explorer = Explorer;
    num_MinerInit = MinerInit;
    num_MinerFinal = MinerFinal;
    num_Adjustment = Adjustment;
    num_rho = rho;
    num_mf = mf;
    num_mc = mc;
    num_alpha = Alpha;
    num_beta = Beta;
    num_archive = Archive;

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
    show.PrintToFileDouble("./result/result" + to_string(Func) + "_DIM" + to_string(num_Dim) + "_NetLen" + to_string(num_Netlen) + "_Eval" + to_string(num_Fess) + ".txt", num_Fess);
    cout << "end" << endl;

}

void SNO::Init()
{
    A.clear();
    show.init();
    best_value = 0;
    eval_count = 0;
    
    Init_T_point(s, num_Explorer, num_Dim, true);
    Init_T_point(x, num_Miner   , num_Dim, true);
    Init_T_point(A, num_archive , num_Dim, false);
    Init_T_point(Points         , num_Point, num_Dim, true);
    Init_T_point(Points_previous, num_Point, num_Dim, false);
    Init_T_net(Net, num_Region, num_Netlen-1);

    SF.resize(num_Point);
    SCR.resize(num_Point);
    SRegion.resize(num_Region);
    deltaF.resize(num_Point);
    
}

void SNO::Evaluation()
{
    while (eval_count < num_Eval)
    {
        SCR.clear();
        SF.clear();
        SRegion.clear();
        deltaF.clear();

        expectedValue();
        regionSearch();
        pointSearch();
        spaceNetAdjustment();
        populationAdjustment();
        updateBestSoFar();
    }
}

void SNO::expectedValue(){ // TODO check δ
    
    // compute each net's expected value
    vector<double> visit_ratio(num_Region, 0);
    vector<double> increase_ratio(num_Region, 0);
    vector<double> EV(num_Region, 0);

    for (int i = 0; i < num_Region; i++) {
        visit_ratio[i] = Net[i]._Vb / Net[i]._Va;

        double increase_sum = 0.0;
        for (int j = 0; j < 4; j++) {
            increase_sum += Points[Net[i].point_index[j]]._fitness - Points_previous[Net[i].point_index[j]]._fitness;
        }
        increase_ratio[i] = increase_sum / 4.0;

        EV[i] = Net[i]._EVbest;
    }

    // min-max normalization of three items
    double max_visit_ratio = *max_element(visit_ratio.begin(), visit_ratio.end());
    double min_visit_ratio = *min_element(visit_ratio.begin(), visit_ratio.end());
    double max_increase_ratio = *max_element(increase_ratio.begin(), increase_ratio.end());
    double min_increase_ratio = *min_element(increase_ratio.begin(), increase_ratio.end());
    double max_EV = *max_element(EV.begin(), EV.end());
    double min_EV = *min_element(EV.begin(), EV.end());
    double omega = 1.5 - (0.5*eval_count)/num_Fess;
    
    for (int i = 0; i < num_Region; i++)
    {
        if (max_visit_ratio - min_visit_ratio == 0)
            visit_ratio[i] = 1;
        else
            visit_ratio[i] = (visit_ratio[i] - min_visit_ratio) / (max_visit_ratio - min_visit_ratio);
        if (max_increase_ratio - min_increase_ratio == 0)
            increase_ratio[i] = 1;
        else
            increase_ratio[i] = (increase_ratio[i] - min_increase_ratio) / (max_increase_ratio - min_increase_ratio);
        if (max_EV - min_EV == 0)
            EV[i] = 1;
        else
            EV[i] = 1 - (EV[i] - min_EV) / (max_EV - min_EV); // due to minimize problem
        // compute expected value
        Net[i]._EV = visit_ratio[i] + increase_ratio[i] + EV[i] * omega;
    }
}

void SNO::regionSearch(){
    // update every point in s
    vector<T_Point> V; // v for mutation & crossover
    Init_T_point(V, num_Explorer, num_Dim, false);

    vector<double> select_region;
    vector<double> select_point;
    select_region.resize(num_Explorer);
    select_region.resize(num_Explorer);
    // update each explorer
    for (int k=0; k<num_Explorer; k++){
        // roulette selection
        vector<T_Net> rouletteRegion = Net;
        // sort first
        sort(rouletteRegion.begin(), rouletteRegion.end(), NetCompareFitness);
        // calculate access regions
        int num_access = (1.0-0.1)*(eval_count/num_Fess) + 0.1;
        double sumEV = 0;
        for (int i = 0; i < num_access; i++)
            sumEV += rouletteRegion[i]._EV;
        double tmp = tool.rand_double(0, sumEV);
        for (int i=0; i<num_access; i++){
            tmp -= rouletteRegion[i]._EV;
            if (tmp <= 0){
                select_region[k] = rouletteRegion[i].net_index;
                break;
            }
        }
        // update visited
        Net[select_region[k]]._Va ++;
        // CR
        Net[select_region[k]]._mC = tool.rand_cauchy(Net[select_region[k]]._mC, 0.1);
        if (Net[select_region[k]]._mC > 1)
            Net[select_region[k]]._mC = 1;
        else if (Net[select_region[k]]._mC < 0)
            Net[select_region[k]]._mC = 0;
        // F
        do {
            Net[select_region[k]]._mF = tool.rand_cauchy(Net[select_region[k]]._mF, 0.1);
            if (Net[select_region[k]]._mF >= 1)
                Net[select_region[k]]._mF = 1;
        } while (Net[select_region[k]]._mF <= 0);
        // Select a point in the region to explore by roulette or best point in region
        vector<T_Point> tofindPoint;
        Init_T_point(tofindPoint, 4, num_Dim, false);
        for (int i=0; i<4; i++)
            tofindPoint[i] = Points[Net[select_region[k]].point_index[i]];
        sort(tofindPoint.begin(), tofindPoint.end(), PointCompareFitness);
        // roulette
        if (tool.rand_double(0,1) < (0.1-1.0)*(eval_count/num_Fess) + 1.0){
            int num_access = (0.1-1.0)*(eval_count/num_Fess) + 1.0;
            double sumPointFit = 0;
            for (int i = 0; i < num_access; i++)
                sumPointFit += tofindPoint[i]._fitness;
            double tmp = tool.rand_double(0, sumPointFit);
            for (int i=0; i<num_access; i++){
                tmp -= tofindPoint[i]._fitness;
                if (tmp <= 0){
                    select_point[k] = tofindPoint[i]._index;
                    break;
                }
            }
        }
        // select best point in net
        else {
            select_point[k] = tofindPoint[0]._index;
        }
    }
    for (int i=0; i<num_Explorer; i++){
        // Mutation & Crossover
        int jrand = tool.rand_int(0, num_Dim - 1);
        for (int j=0; j<num_Dim; j++){
            if (tool.rand_double(0,1) < Net[select_region[i]]._mC || i == jrand){
                int r2 = 0;          
                do{
                    r2 = tool.rand_int(0, (num_Explorer - 1) + (A.size() - 1));
                } while (r2==i);
                
                if (tool.rand_double(0,1) < powf(eval_count/num_Eval, num_alpha)){
                    int r1 = 0;
                    do {
                        r2 = tool.rand_int(0, num_Explorer - 1);
                    } while (r1 == i || r1 == r2);
                    // mutation
                    if (r2<num_Explorer)
                        V[i]._position[j] = Points[select_point[i]]._position[j] +
                                            Net[select_region[i]]._mF * (s[r1]._position[j] - s[r2]._position[j]);
                    else
                        V[i]._position[j] = s[i]._position[j] +
                                            Net[select_region[i]]._mF * (s[r1]._position[j] - A[r2-num_Explorer]._position[j]);
                }
                else {
                    V[i]._position[j] = s[i]._position[j] +
                                            Net[select_region[i]]._mF * (Points[select_point[i]]._position[j] - A[r2-num_Explorer]._position[j]);
                }
            }
            else {
                V[i]._position[j] = s[i]._position[j];
            }
            // check boundary
            if (V[i]._position[j] < problem.getBounderMin() || V[i]._position[j] > problem.getBounderMax())
                V[i]._position[j] = tool.rand_double(problem.getBounderMin(), problem.getBounderMax());
            // update fitness
            V[i]._fitness = problem.executeStrategy(V[i]._position, num_Dim);
            eval_count++;
        }
        // update s & A : 10-18
        if (V[i]._fitness < s[i]._fitness){
            SF.push_back(Net[select_region[i]]._mF);
            SCR.push_back(Net[select_region[i]]._mC);
            SRegion.push_back(select_region[i]);
            deltaF.push_back(s[i]._fitness - V[i]._fitness);
            if (A.size() < num_archive){
                A.push_back(s[i]);
            }
            else {
                int r = tool.rand_int(0, num_archive-1);
                A[r] = s[i];
            }
            s[i] = V[i];
        }
        updateBestSoFar();
    }
    // algo16 : line 20
    spaceNetAdjustment(); // TODO
    // update vb
    for (int i=0; i<num_Region; i++){
        bool flag = false;
        for (int j=0; j<select_point.size(); j++)
            if (i == select_point[j]){
                flag = true;
                break;
            }
        // 沒被搜尋過的VB+1 , 論文中未提及搜尋過該如何處理 , 直接用VN的方式更新
        if (flag) {
            Net[i]._Va ++;
            Net[i]._Vb = 1;
        }
        else {
            Net[i]._Va = 1;
            Net[i]._Vb ++;
        }
        // update mc & mf
        if (SCR.size() != 0 && SF.size() != 0) {
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

            // Update MF
            for (int t = 0; t < SCR.size(); t++)
            {
                // Lehmer mean
                numerator += (deltaF[t] / WKdenominator) * SF[t] * SF[t];
                denominator += (deltaF[t] / WKdenominator) * SF[t];
            }
            mF = numerator / denominator;
            Net[i]._mC = mCR;
            Net[i]._mF = mF;
        }
    }
}

void SNO::pointSearch(){

}

void SNO::spaceNetAdjustment(){

}

void SNO::populationAdjustment(){

}


void SNO::updateBestSoFar(){
    double best = Points[0]._fitness;
    for (int i=1; i<num_Point; i++){
        if (best > Points[i]._fitness){
            best = Points[i]._fitness;
        }
    }
    for (int i = 0; i < num_Explorer; i++){
        if (best > s[i]._fitness){
            best = s[i]._fitness;
        }
    }
    for (int i = 0; i < num_Miner; i++){
        if (best > x[i]._fitness){
            best = x[i]._fitness;
        }
    }
}

// DS Tool
bool SNO::NetCompareFitness(const T_Net &a, const T_Net &b)
{
    // return descending order
    return a._EV > b._EV;
}

bool SNO::PointCompareFitness(const T_Point &a, const T_Point &b)
{
    // return ascending order
    return a._fitness < b._fitness;
}

void SNO::Init_T_point(vector<T_Point> &points, int size, int dim, bool posInitRand){
    points.resize(size);
    for (int i=0; i<size; i++){
        points[i]._index = i;
        points[i]._position.assign(dim, 0);
        points[i]._inCR = points[i]._inF = 0;
        points[i]._fitness = 0;
        if (posInitRand){
            for (int j = 0; j < dim; j++)
                points[i]._position[j] = tool.rand_double(problem.getBounderMin(), problem.getBounderMax());
            points[i]._fitness = problem.executeStrategy(points[i]._position, num_Dim);
        }
    }
}

void SNO::Init_T_net(vector<T_Net> &Net, int size, int num_region){
    Net.resize(size);
    for (int i=0; i<num_region; i++){
        for (int j=0; j<num_region; j++){
            Net[i*(num_region)+j].net_index = i*(num_region)+j;
            Net[i*(num_region)+j].point_index.assign(4, 0);
            Net[i*(num_region)+j].point_index[0] = i*num_Netlen + j;
            Net[i*(num_region)+j].point_index[1] = i*num_Netlen + j + 1;
            Net[i*(num_region)+j].point_index[2] = (i+1)*num_Netlen + j;
            Net[i*(num_region)+j].point_index[3] = (i+1)*num_Netlen + j + 1;
            Points[i*num_Netlen + j]._netIndex = Points[i*num_Netlen + j + 1]._netIndex = Points[(i+1)*num_Netlen + j]._netIndex = Points[(i+1)*num_Netlen + j + 1]._netIndex = i*(num_region)+j;
            Net[i*(num_region)+j]._Va = Net[i*(num_region)+j]._Vb = 1;
            Net[i*(num_region)+j]._EV = 0;
            Net[i*(num_region)+j]._mC = num_mc;
            Net[i*(num_region)+j]._mF = num_mf;
            
            // calculate Ebest TODO check useful
            Net[i*(num_region)+j]._EVbest  = Points[Net[i*(num_region)+j].point_index[0]]._fitness;
            eval_count++;
            for (int k=1; k<4; k++){
                if (Net[i*(num_region)+j]._EVbest > Points[Net[i*(num_region)+j].point_index[k]]._fitness){
                    Net[i*(num_region)+j]._EVbest = Points[Net[i*(num_region)+j].point_index[k]]._fitness;
                }
            }
        }
    }
}

# endif