#ifndef SNO_H
#define SNO_H
// #include "../AlgPrint.h"
#include "../Tool.h"
#include "./cec2021.cpp" // TODO windows.h, change to linux os
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
        double _EV, _EVbest;
    } T_Net;

public:
    void RunALG(int, int, int, int, int, int, int, double, double, double, double, double, double, double, double);
private:
    //env
    int num_Run;
    int num_Dim;
    int num_Eval;
    int num_Netlen;
    int num_Explorer;
    int num_MinerInit;
    int num_MinerRate;
    int num_Adjustment;
    double num_rho;
    double num_mf;
    double num_mc;
    double num_alpha;
    double num_beta;
    double num_archiveRate;
    double num_archive;

    // parameter of algorithm
    int num_Miner;
    vector<T_Point> s, x; // s: explorer, x: miner
    vector<T_Point> Points, Points_previous;
    vector<T_Net> Net;
    vector<double> SF, SCR, deltaF;
    vector<T_Point> Archives;

    // var
    int eval_count, show_count;
    int num_Point;
    int num_Population;
    int num_Region;

    double best_value;

    // tool
    // AlgPrint show;
    Tool tool;
    Problem problem;

    // method
    void Init();
    void Evaluation();

    void expectedValue();
    void regionSearch();
    void pointSearch();
    void spaceNetAdjustment(T_Point &);
    void populationAdjustment(int init_size);
    void updateBestSoFar();

    // DS
    void Init_T_point(T_Point &point, int index, int dim, bool posInitRand);
    void Init_T_points(vector<T_Point> &points, int size, int dim, bool posInitRand);
    void Init_T_net(vector<T_Net> &nets, int size, int num_region);

    static bool NetCompareFitness(const T_Net &a, const T_Net &b);
    int NetSelectTopPBest(vector<T_Net> xx, double p);
    static bool PointCompareFitness(const T_Point &a, const T_Point &b);
    void calDis(vector<double> &a, vector<double> &b, double &dis);
    void rankDis(T_Point &target, vector<T_Point> &points, vector<int> &rank); // rank distance between target and points, store rank in store 

};
// Run, Func, Evals, Dim, NetLength, Explorer, MinerInit, MinerRate, AdjustmentMax, Rho, mf, mc, Alpha, Beta, Archive
void SNO::RunALG(int Run, int Func, int Eval, int Dim, 
                 int NetLength, int Population, int Adjustment, double MinerInit, double MinerRate, 
                 double rho, double mf, double mc, double Alpha, double Beta, double Archive)
{
    num_Run = Run;
    num_Eval = Eval;
    num_Dim = Dim;
    num_Netlen = NetLength;
    num_Population = Population;
    num_MinerInit = MinerInit;
    num_MinerRate = MinerRate;
    num_Adjustment = Adjustment;
    num_rho = rho;
    num_mf = mf;
    num_mc = mc;
    num_alpha = Alpha;
    num_beta = Beta;
    num_archiveRate = Archive;

    // show = AlgPrint(num_Run, "./result", "sno");
    // show.NewShowDataDouble(num_Eval);

    problem.setStrategy(Func);

    while (num_Run--)
    {
        cout << "-------------------Run" << Run - num_Run << "---------------------" << endl;
        Init();
        cout << "init" << endl;
        Evaluation();
        cout << "eval" << endl;
    }
    // show.PrintToFileDouble("./result/result" + to_string(Func) + "_DIM" + to_string(num_Dim) + "_NetLen" + to_string(num_Netlen) + "_Eval" + to_string(num_Eval) + ".txt", show_count);
    cout << "end" << endl;

}

void SNO::Init()
{
    Archives.clear();
    // show.init();
    eval_count = show_count = 0;
    
    num_Region = (num_Netlen-1)*(num_Netlen-1);
    num_Point = num_Netlen*num_Netlen;
    
    num_archive = num_archiveRate * num_Population;
    num_Explorer = num_Population;
    num_Miner = num_MinerInit * num_Population;

    Init_T_points(s, num_Explorer, num_Dim, true);
    Init_T_points(x, num_Explorer, num_Dim, true);
    Init_T_points(Archives, num_archive, num_Dim, false);
    Init_T_points(Points         , num_Point, num_Dim, true);
    Init_T_points(Points_previous, num_Point, num_Dim, false);
    Init_T_net(Net, num_Region, num_Netlen-1);
    
    best_value = __DBL_MAX__;
    updateBestSoFar();    

    SF.resize(num_Point);
    SCR.resize(num_Point);
    deltaF.resize(num_Point);
}

void SNO::Evaluation()
{
    bool flag = false;
    while (eval_count < num_Eval)
    {
        SCR.clear();
        SF.clear();
        deltaF.clear();
        
        if (flag)
            Points_previous = Points;
        flag = true;
        expectedValue();
        regionSearch();
        pointSearch();
        populationAdjustment(num_Population);
        updateBestSoFar();

        cout << "run: " << show_count << endl;
        if (eval_count>=num_Eval)
            break;
        else {
            // show.SetDataDouble(num_Run, best_value, show_count);
            show_count = eval_count;
            cout << "final: " << num_Eval << " ,Eval: " << show_count << ", " << best_value << endl;
        }
    }
    cout << "FINISH: " << show_count << endl;
}
void SNO::expectedValue(){   
    // compute each net's expected value
    vector<double> visit_ratio(num_Region, 0);
    vector<double> increase_ratio(num_Region, 0);
    vector<double> EV(num_Region, 0);

    for (int i = 0; i < num_Region; i++) {
        visit_ratio[i] = Net[i]._Vb / Net[i]._Va;

        double increase_sum = 0.0;
        for (int j = 0; j < 4; j++) {
            // calculate increase sum
            increase_sum += Points[Net[i].point_index[j]]._fitness - Points_previous[Net[i].point_index[j]]._fitness;
            // update best in net
            if (Net[i]._EVbest > Points[Net[i].point_index[j]]._fitness)
                Net[i]._EVbest = Points[Net[i].point_index[j]]._fitness;
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
    double omega = 2 - (double)eval_count/num_Eval;
    
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
    // release memory
    vector<double>().swap(visit_ratio);
    vector<double>().swap(increase_ratio);
    vector<double>().swap(EV);
}

void SNO::regionSearch(){
    // update every point in s
    vector<T_Point> V; // v for mutation & crossover // TODO if the mf & mc passwthrough from last generation
    Init_T_points(V, num_Explorer, num_Dim, false);
    double cr, f;
    vector<int> select_region(num_Explorer, 0);
    vector<int> select_point(num_Explorer, 0);
    // update each explorer
    for (int i=0; i<num_Explorer; i++) {
        int num_access = (1.0-0.1)*((double)eval_count/num_Eval) + 0.1;
        // roulette selection
        vector<T_Net> rouletteRegion = Net;
        // sort first
        sort(rouletteRegion.begin(), rouletteRegion.end(), NetCompareFitness);
        if (num_access==0){
            select_region[i] = rouletteRegion[0].net_index;
        }
        else {
            // calculate access regions
            double sumEV = 0;
            for (int i = 0; i < num_access; i++) {
                sumEV += rouletteRegion[i]._EV;
            }
            double tmp = tool.rand_double(0, sumEV);
            for (int i=0; i<num_access; i++){
                tmp -= rouletteRegion[i]._EV;
                if (tmp <= 0){
                    select_region[i] = rouletteRegion[i].net_index;
                    break;
                }
            }
        }
        vector<T_Net>().swap(rouletteRegion);
        // update visited // algo 16 : line 4
        Net[select_region[i]]._Va ++;
        // algo 16 : line 5
        // CR
        cr = tool.rand_cauchy(Net[select_region[i]]._mC, 0.1);
        if (cr > 1)
            cr = 1;
        else if (cr < 0)
            cr = 0;
        // F
        do {
            f = tool.rand_cauchy(Net[select_region[i]]._mF, 0.1);
            if (f >= 1)
                f = 1;
        } while (f <= 0);
        // algo 16 : line 6
        // Select a point in the region to explore by roulette or best point in region
        vector<T_Point> tofindPoint;
        Init_T_points(tofindPoint, 4, num_Dim, false);
        for (int i=0; i<4; i++)
            tofindPoint[i] = Points[Net[select_region[i]].point_index[i]];
        sort(tofindPoint.begin(), tofindPoint.end(), PointCompareFitness);
        // Tournament // algo 16 : line 6 : formula 3.7
        if (tool.rand_double(0,1) < (0.1-1.0)*((double)eval_count/num_Eval) + 1.0){
            int r1, r2;
            r1 = tool.rand_int(0, 3);
            do {
                r2 = tool.rand_int(0, 3);
            } while (r1 == r2);
            if (r1 > r2)
                select_point[i] = Net[select_region[i]].point_index[r2];
            else
                select_point[i] = Net[select_region[i]].point_index[r1];
        }
        // select best point in net
        else
            select_point[i] = tofindPoint[0]._index;
        vector<T_Point>().swap(tofindPoint);
        // algo 16 : line 7
        // Mutation & Crossover
        int jrand = tool.rand_int(0, num_Dim - 1);
        for (int j=0; j<num_Dim; j++){
            if (tool.rand_double(0,1) < cr || j == jrand){
                int r2;
                do{
                    r2 = tool.rand_int(0, (num_Explorer - 1) + (Archives.size() - 1));
                } while (r2==i);
                if (tool.rand_double(0,1) < powf((double)eval_count/num_Eval, num_alpha)){
                    int r1;
                    do {
                        r1 = tool.rand_int(0, num_Explorer - 1);
                    } while (r1 == i || r1 == r2);
                    // mutation
                    if (r2<num_Explorer)
                        V[i]._position[j] = Points[select_point[i]]._position[j] +
                                            f * (s[r1]._position[j] - s[r2]._position[j]);
                    else
                        V[i]._position[j] = Points[select_point[i]]._position[j] +
                                            f * (s[r1]._position[j] - Archives[r2-num_Explorer]._position[j]);
                }
                else {
                    if (r2<num_Explorer) {
                        V[i]._position[j] = s[i]._position[j] +
                                            f * (Points[select_point[i]]._position[j] - s[r2]._position[j]);
                    }
                    else
                        V[i]._position[j] = s[i]._position[j] +
                                            f * (Points[select_point[i]]._position[j] - Archives[r2-num_Explorer]._position[j]);
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
            deltaF.push_back(s[i]._fitness - V[i]._fitness);
            if (Archives.size() < num_archive)
                Archives.push_back(s[i]);
            else
                Archives[tool.rand_int(0, num_archive-1)] = s[i];
            s[i]._position = V[i]._position;
            s[i]._fitness = V[i]._fitness;
        }
    }
    // algo16 : line 19
    updateBestSoFar();
    // algo16 : line 20
    for (int i=0; i<num_Explorer; i++)
        spaceNetAdjustment(V[i]);
    // release memory
    vector<T_Point>().swap(V);
    vector<int>().swap(select_region);
    vector<int>().swap(select_point);
    // update vb
    for (int i=0; i<num_Region; i++){
        bool flag = false;
        for (int j=0; j<select_point.size(); j++)
            if (i == select_point[j]){
                flag = true;
                break;
            }
        // 沒被搜尋過的VB+1 , 論文中未提及搜尋過該如何處理 , 直接用VN的方式更新 // TODO double check
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
            for (int t = 0; t < SCR.size(); t++) {
                // mean weight Scr
                mCR += (deltaF[t] / WKdenominator) * SCR[t];
            }

            // Update MF
            for (int t = 0; t < SCR.size(); t++) {
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

void SNO::pointSearch() {
    for (int i=0; i<num_Miner; i++){
        // Rank all net points
        vector<T_Point> tmpPoints = Points;
        sort(tmpPoints.begin(), tmpPoints.end(), PointCompareFitness);
        // select point
        int num_access = ((0.1-num_rho)*((double)eval_count/num_Eval) + num_rho) * num_Point;
        int select_NSpoint = tool.rand_int(0, num_access-1);
        // select a region to get mf, mc
        int select_region = tool.rand_int(0, num_Region-1);
        // Generate CR & F
        double cr, f;
        // CR // TODO pack to function
        cr = tool.rand_cauchy(Net[select_region]._mC, 0.1);
        if (cr > 1)
            cr = 1;
        else if (cr < 0)
            cr = 0;
        // F
        do {
            f = tool.rand_cauchy(Net[select_region]._mF, 0.1);
            if (f >= 1)
                f = 1;
        } while (f <= 0);
        // tmp variable
        T_Point V; // v for mutation & crossover
        Init_T_point(V, 1, num_Dim, false);
        // randomly select a miner
        int select_miner = tool.rand_int(0, num_Miner-1);
        // Generate new solution
        int jrand = tool.rand_int(0, num_Dim - 1);
        for (int j=0; j<num_Dim; j++){
            // mutation & crossover
            if (tool.rand_double(0,1) < cr || j == jrand) {
                int r1, r2;
                do {
                    r1 = tool.rand_int(0, num_Miner - 1);
                } while (r1==i);
                do {
                    r2 = tool.rand_int(0, num_Miner - 1);
                } while (r2==r1 || r2==i);
                
                if (tool.rand_double(0,1) < powf((double)eval_count/num_Eval, num_beta))
                    V._position[j] = Points[select_NSpoint]._position[j] + f * (x[r1]._position[j] - x[r2]._position[j]);
                else
                    V._position[j] = x[select_miner]._position[j] + f * (tmpPoints[r1]._position[j] - x[r2]._position[j]);
            }
            else
                V._position[j] = x[select_miner]._position[j];
            // check boundary
            if (V._position[j] < problem.getBounderMin() || V._position[j] > problem.getBounderMax())
                V._position[j] = tool.rand_double(problem.getBounderMin(), problem.getBounderMax());
        }
        vector<T_Point>().swap(tmpPoints);
        // update fitness
        V._fitness = problem.executeStrategy(V._position, num_Dim);
        eval_count++;
        if (x[select_miner]._fitness > V._fitness){
            x[select_miner]._position = V._position;
            x[select_miner]._fitness = V._fitness;
        }
        // update net
        spaceNetAdjustment(V);
        // update best so far
        updateBestSoFar();
    }
}

void SNO::spaceNetAdjustment(T_Point &search){
    vector<int> rank(num_Point, 0);
    rankDis(search, Points, rank);
    int num_access = max((int)round( (num_Point * num_Adjustment - 1) * ((double)eval_count/num_Eval)) + 1, 1);
    // prepare for mutation & crossover
    vector<vector<double>> prepare;
    prepare.resize(2);
    prepare[0].assign(num_Dim, 0);
    prepare[1].assign(num_Dim, 0);
    for (int i=0; i<num_access; i++){
        int rand_index = tool.rand_int(0, num_access-1);
        int rand_region = tool.rand_int(0, num_Region-1);
        double cr, f;
        // CR
        cr = tool.rand_cauchy(Net[rand_region]._mC, 0.1);
        if (cr > 1)
            cr = 1;
        else if (cr < 0)
            cr = 0;
        // F
        do {
            f = tool.rand_cauchy(Net[rand_region]._mF, 0.1);
            if (f >= 1)
                f = 1;
        } while (f <= 0);
        int r1, r2;
        do {
            r1 = tool.rand_int(0, (num_Explorer-1) + (num_Miner-1));
        } while (r1 == rand_index);
        do {
            r2 = tool.rand_int(0, (num_Explorer-1) + (num_Miner-1));
        } while (r2 == rand_index || r2 == r1);
        // prepare for mutation & crossover
        for (int j=0; j<num_Dim; j++){
            double pos_r1 = 0, pos_r2 = 0;
            if (r1 < num_Explorer)
                pos_r1 = s[r1]._position[j];
            else
                pos_r1 = x[r1-num_Explorer]._position[j];
            if (r2 < num_Explorer)
                pos_r2 = s[r2]._position[j];
            else
                pos_r2 = x[r2-num_Explorer]._position[j];
            // prepare n1
            prepare[0][j] = search._position[j] + f * (pos_r1 - pos_r2);
            // prepare n2
            prepare[1][j] = Points[rank[i]]._position[j] 
                          + f * (search._position[j] - Points[rank[i]]._position[j])
                          + f * (pos_r1 - pos_r2);
        }
        // select N'a,i to update
        T_Point select_netPoint;
        select_netPoint._position.assign(num_Dim, 0);
        rank.assign(num_Point, 0);
        if (i==0){
            int rand_index = tool.rand_int(0, (num_Explorer-1) + (num_Miner-1));
            if (rand_index < num_Explorer)
                select_netPoint = s[rand_index];
            else
                select_netPoint = x[rand_index-num_Explorer];
            select_netPoint._fitness = problem.executeStrategy(select_netPoint._position, num_Dim);
            eval_count++;
        }
        else if (tool.rand_double(0,1) < ((double)eval_count/num_Eval)) {
            double dis0, dis1;
            calDis(prepare[0], search._position, dis0);
            calDis(prepare[1], search._position, dis1);
            if (dis0 < dis1)
                select_netPoint._position = prepare[0];
            else
                select_netPoint._position = prepare[1];
        }
        else {
            double dis0, dis1;
            calDis(prepare[0], Points[rank[i]]._position, dis0);
            calDis(prepare[1], Points[rank[i]]._position, dis1);
            if (dis0 < dis1)
                select_netPoint._position = prepare[0];
            else
                select_netPoint._position = prepare[1];
        }
        // adjust space net
        rankDis(select_netPoint, Points, rank);
        if (select_netPoint._fitness < Points[rank[0]]._fitness) {
            Points[rank[0]]._position = select_netPoint._position;
            Points[rank[0]]._fitness = select_netPoint._fitness;
        }
    }
    vector<int>().swap(rank);
    vector<vector<double>>().swap(prepare);
}

void SNO::populationAdjustment(int init_size){
    int min_size = 10;
    double change_rate = sqrt((double)eval_count/num_Eval);
    num_Population = (double)(min_size - init_size) * pow(change_rate, (1.0-change_rate)) + init_size;
    // remove explorer
    vector<T_Point> tmp = s;
    while(s.size() > num_Population) {
        sort(tmp.begin(), tmp.end(), PointCompareFitness);
        reverse(tmp.begin(), tmp.end()); // worst at first
        s.erase(s.begin() + tmp[0]._index);
    }
    vector<T_Point>().swap(tmp);
    num_Explorer = s.size();

    // remove archive
    while (Archives.size() > round(num_archiveRate*num_Population)) {
        int rand_index = (int)round(tool.rand_double(0,1) * (Archives.size() - 1));
        Archives.erase(Archives.begin() + rand_index);
    }
    num_archive = Archives.size();
    
    double p_best = (0.1 - num_rho) * ((double) eval_count / num_Eval) + num_rho;
    int s_init_size = num_Miner;
    int s_new_size = (int)round(num_MinerRate * pow(change_rate, 1.0 - change_rate) * init_size + s_init_size);
    vector<T_Point> rankPoints = Points;
    sort(rankPoints.begin(), rankPoints.end(), PointCompareFitness);

    // add new miner
    T_Point new_miner;
    while (num_Miner < s_new_size){
        int rand_index = (int)round(tool.rand_double(0,1) * p_best * (num_Point - 1));
        Init_T_point(new_miner, num_Miner, num_Dim, true);
        for(int i=0; i<num_Dim; ++i){
            if (tool.rand_double(0,1) < 0.5){
                double rand_value = (problem.getBounderMax() - problem.getBounderMin()) * tool.rand_double(0,1) + problem.getBounderMin();
                new_miner._position[i] = num_alpha * Points[rankPoints[rand_index]._index]._position[i] + (1 - num_alpha) * rand_value;
            }
            else {
                new_miner._position[i] = Points[rankPoints[rand_index]._index]._position[i];
            }
        }
        new_miner._fitness = problem.executeStrategy(new_miner._position, num_Dim);
        eval_count++;
        if (new_miner._fitness < best_value){
            best_value = new_miner._fitness;
            // replace the worst point in s
            vector<T_Point> worsts = s;
            sort(worsts.begin(), worsts.end(), PointCompareFitness);
            reverse(worsts.begin(), worsts.end());
            s[worsts[0]._index]._position = new_miner._position;
            s[worsts[0]._index]._fitness  = new_miner._fitness;
            vector<T_Point>().swap(worsts);
        }
        x.push_back(new_miner);
        num_Miner = x.size();
    }
    vector<T_Point>().swap(rankPoints);
}

void SNO::updateBestSoFar(){
    for (int i=0; i<num_Point; i++)
        if (best_value > Points[i]._fitness){
            best_value = Points[i]._fitness;
        }

    for (int i = 0; i < num_Explorer; i++)
        if (best_value > s[i]._fitness){
            best_value = s[i]._fitness;
        }
            
    for (int i = 0; i < num_Miner; i++)
        if (best_value > x[i]._fitness){
            best_value = x[i]._fitness;
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

void SNO::calDis(vector<double> &a, vector<double> &b, double &dis){
    dis = 0;
    for (int i=0; i<num_Dim; i++){
        dis += pow(a[i] - b[i], 2);
    }
    dis = sqrt(dis);
}

void SNO::rankDis(T_Point &target, vector<T_Point> &points, vector<int> &rank){
    vector<T_Point> tmPoints;
    tmPoints.resize(points.size());
    tmPoints = points;
    // calculate distance
    for (int i=0; i<points.size(); i++){
        calDis(target._position, tmPoints[i]._position, tmPoints[i]._fitness);
    }
    // rank distance and store point_index in rank
    sort(tmPoints.begin(), tmPoints.end(), PointCompareFitness);
    for (int i=0; i<tmPoints.size(); i++){
        rank[i] = tmPoints[i]._index;
    }
    // release memory
    vector<T_Point>().swap(tmPoints);
}

void SNO::Init_T_points(vector<T_Point> &points, int size, int dim, bool posInitRand) {
    points.resize(size);
    for (int i = 0; i < size; i++) {
        Init_T_point(points[i], i, dim, posInitRand);
    }
}

void SNO::Init_T_point(T_Point &point, int index, int dim, bool posInitRand){
    point._index = index;
    point._position.assign(dim, 0);
    point._inCR = num_mc;
    point._inF = num_mf;
    point._fitness = 0;
    if (posInitRand){
        for (int i = 0; i < dim; i++)
            point._position[i] = tool.rand_double(problem.getBounderMin(), problem.getBounderMax());
        point._fitness = problem.executeStrategy(point._position, num_Dim);
        eval_count++;
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
            Net[i*(num_region)+j]._EV = __DBL_MAX__;
            Net[i*(num_region)+j]._EVbest = __DBL_MAX__;
            Net[i*(num_region)+j]._mC = num_mc;
            Net[i*(num_region)+j]._mF = num_mf;
        }
    }
}

# endif