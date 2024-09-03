#ifndef SE_H
#define SE_H

#include "../AlgPrint.h"
#include "../Tool.h"
#include "./OneMax.cpp"
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class SE
{
public:
    typedef vector<int> i1d;
    typedef vector<i1d> i2d;
    typedef vector<i2d> i3d;
    typedef vector<i3d> i4d;
    typedef vector<double> d1d;
    typedef vector<d1d> d2d;
    typedef vector<d2d> d3d;

public:
    void RunALG(int, int, int, int, int, int, int, int);

private:
    // env
    int num_Run;
    int num_Fess;
    int num_Dim;
    int num_Searchers;
    int num_Regions;
    int num_Samples;
    int num_Players;
    int num_Evals;

    // search result
    int Best;
    i1d Best_sol;

    // search algorithm
    i2d searcher_sol;  // [searcher, bit_dim]
    i3d sample_sol;    // [region, sample, bit_dim]
    i2d sample_best;   // [region, bit_dim]
    i4d sampleV_sol;   // [searcher, region, sample, bit_dim]

    d1d searcher_sol_fit;
    d2d sample_sol_fit; // [region, sample]
    d1d sample_best_fit;
    d3d sampleV_sol_fit;

    d1d ta; // number of times the j-th region has been invested
    d1d tb; // number of times the j-th region has not been invested

    d2d expected_value; // [searcher, region]
    d1d T_j; // 第j個區域的被搜尋頻率( ta[j]) 與未被搜尋的次數( tb[j]) 的比值->區域搜索強度
    d1d M_j; // 第j個區域的最佳解適應度佔所有區域總適應度的比值->區域重要性

    i1d searcher_region_id; // [searcher], region id
    i2d id_bits;        // region id: tabu bits
    int num_id_bits;    // number of bits in id

    int eval_count; // evaluation count

    // tool
    AlgPrint show;
    Tool tool;
    Problem problem;
    
    void init();
    void resource_arrangement();
    void vision_search(int);
    void transit();
    void compute_expected_value(int);
    void vision_selection(int, int);
    void marketing_search(int&);
};


void SE::RunALG(int Run, int Func, int Evals, int Bits, int Searchers, int Regions, int Samples, int Players)
{
    num_Run = Run;
    num_Fess = Evals;
    num_Dim = Bits;
    num_Searchers = Searchers;
    num_Regions = Regions;
    num_Samples = Samples;
    num_Players = Players;


    show = AlgPrint(num_Run, "./result", "se");
    show.NewShowDataInt(num_Fess);

    problem.setStrategy(Func);

    while (num_Run--)
    {
        init();
        resource_arrangement();
        while (eval_count < num_Fess)
        {
            int eval_cc = eval_count;
            vision_search(eval_cc);
            marketing_search(Best);
            show.SetDataInt(num_Run, Best, eval_count);
            cout << "Run: " << num_Run << " eval_count: " << eval_count << " Best: " << Best << endl;
        }
    }
    show.PrintToFileDouble("./result/result" + to_string(Func) + ".txt", num_Fess);
}

void SE::init()
{
    searcher_sol.assign(num_Searchers, i1d(num_Dim, 0));
    sample_sol.assign(num_Regions, i2d(num_Samples, i1d(num_Dim, 0)));
    sample_best.assign(num_Regions, i1d(num_Dim, 0));
    sampleV_sol.assign(num_Searchers, i3d(num_Regions, i2d(num_Samples*2, i1d(num_Dim, 0))));

    searcher_sol_fit.assign(num_Searchers, 0.0);
    sample_sol_fit.assign(num_Regions, d1d(num_Samples, 0.0));
    sample_best_fit.assign(num_Regions, 0.0);
    sampleV_sol_fit.assign(num_Searchers, d2d(num_Regions, d1d(num_Samples, 0.0)));

    Best_sol.assign(num_Dim, 0);
    Best = 0;
    eval_count = 0;

    for (int i=0; i<num_Searchers; i++)
        for (int j=0; j<num_Dim; j++)
            searcher_sol[i][j] = tool.rand_int(0, 1);
}

void SE::resource_arrangement()
{
    // init searchers and regions
    num_id_bits = log2(num_Regions);
    searcher_region_id.assign(num_Searchers, 0);
    id_bits.assign(num_Regions, i1d(num_id_bits, 0));

    // region_id_bits(), divide region and give region id
    for (int i=0; i<num_Regions; i++)
    {
        int n = i;
        int j = num_id_bits;
        while (n > 0) {
            id_bits[i][--j] = n%2;
            n /= 2;
        }
    }

    // assign searchers to regions
    for (int i=0; i<num_Searchers; i++)
    {
        // assign region id to each searcher
        const int r = tool.rand_int(0, num_Regions-1);
        searcher_region_id[i] = r;

        for (int j=0; j < num_id_bits; j++)
            searcher_sol[i][j] = id_bits[r][j];
    }

    // init sample solutions
    for (int i=0; i<num_Regions; i++){
        for (int j=0; j<num_Samples; j++){
            for (int k=0; k<num_Dim; k++)
                sample_sol[i][j][k] = id_bits[i][k];
            for (int k=num_id_bits; k<num_Dim; k++)
                sample_sol[i][j][k] = tool.rand_int(0, 1);
        }
    }

    // init investment and how long region has not been invested
    ta.assign(num_Regions, 0.0);
    tb.assign(num_Regions, 1.0);

    for (int i=0; i<num_Searchers; i++){
        int r = searcher_region_id[i];
        ta[r]++;
        tb[r] = 1.0;
    }

    // init expected value
    expected_value.assign(num_Searchers, d1d(num_Regions, 0.0));
    T_j.assign(num_Regions, 0.0);
    M_j.assign(num_Regions, 0.0);
}

void SE::vision_search(int eval)
{
    // construct sampleV_sol
    if (eval > 0) transit();

    // compute fitness of all regions of all searchers
    compute_expected_value(eval);

    // selecti region to which a search will be assigned
    vision_selection(num_Players, eval);
}

// construct V (searcher X sample)
void SE::transit() {
    // exchange information sample_sol and searcher_sol
    for (int i = 0; i < num_Searchers; i++) {
        for (int j = 0; j < num_Regions; j++) {
            for (int k = 0; k < num_Samples; k++) {
                const int x = tool.rand_int(0, num_Dim+1); // divide place, from start to end(must add 1)
                const int m = k << 1; // m = 2k

                // 設定每個searcher的sampleV_sol的區域id
                for (int l = 0; l < num_id_bits; l++) {
                    sampleV_sol[i][j][m][l]   = id_bits[j][l];
                    sampleV_sol[i][j][m+1][l] = id_bits[j][l];
                }
                // 交換sample_sol與searcher_sol的bit
                for (int l = num_id_bits; l < num_Dim; l++) {
                    // Crossover point X
                    if (l < x) {
                        sampleV_sol[i][j][m][l]   = searcher_sol[i][l];
                        sampleV_sol[i][j][m+1][l] = sample_sol[j][k][l];
                    } else {
                        sampleV_sol[i][j][m][l]   = searcher_sol[i][l];
                        sampleV_sol[i][j][m+1][l] = sample_sol[j][k][l];
                    }
                }
            }
        }
    }

    // randomly change one bit of each solution in sampleV_sol
    for (int i = 0; i < num_Searchers; i++) {
        for (int j = 0; j < num_Regions; j++) {
            for (int k = 0; k < 2*num_Samples; k++) {
                int t = tool.rand_int(0, num_Dim);
                if (t>=num_id_bits) {
                    sampleV_sol[i][j][k][t] = !sampleV_sol[i][j][k][t];
                }
            }
        }
    }
}

void SE::compute_expected_value(int eval){
    // set fitness of all regions of all searchers 
    if (eval == 0) {
        for (int i=0; i< num_Searchers; i++){
            searcher_sol_fit[i] = problem.executeStrategy(searcher_sol[i], num_Dim);
            eval_count++;
        }
    }
    else {
        // fitness of sampleV_sol (new candidate solutions)
        for (int i=0; i<num_Searchers; i++){
            int j = searcher_region_id[i];
            for (int k=0; k<num_Samples; k++){
                int n = tool.rand_int(0, 2*num_Samples-1);
                int f = problem.executeStrategy(sampleV_sol[i][j][n], num_Dim);
                eval_count++;

                // 更新搜尋者的解和適應值
                if (f > searcher_sol_fit[i]){
                    searcher_sol[i] = sampleV_sol[i][j][n];
                    searcher_sol_fit[i] = f;
                }
                // 更新樣本的解和適應值
                if (f > sample_sol_fit[j][k]){
                    sample_sol[j][k] = sampleV_sol[i][j][n];
                    sample_sol_fit[j][k] = f;
                }
            }
        }
    }

    // fitness value of samples
    if (eval == 0)
        for (int j=0; j < num_Regions; j++)
            for (int k=0; k < num_Samples; k++){
                sample_sol_fit[j][k] = problem.executeStrategy(sample_sol[j][k], num_Dim);
                eval_count++;
            }

    double total_sample_fitness = 0.0; // f(m_j)
    for (int j=0; j<num_Regions; j++){
        double region_best_fit = 0.0; // 區域最佳化適應值
        int b = -1; // index of best sample

        for (int k=0; k < num_Samples; k++){
            total_sample_fitness += sample_sol_fit[j][k];
            // update best sample
            if (sample_sol_fit[j][k] > region_best_fit){
                b = k;
                region_best_fit = sample_sol_fit[j][k];
            }
        }
        if (b>=0){
            sample_best_fit[j] = region_best_fit;
            sample_best[j] = sample_sol[j][b];
        }
    }

    // M_J
    for (int j=0; j < num_Regions; j++){
        M_j[j] = sample_best_fit[j] / total_sample_fitness;
    }
    // T_J
    for (int j = 0; j< num_Regions; j++)
        T_j[j] = ta[j] / tb[j];

    // compute expected value
    for (int i = 0; i < num_Searchers; i++)
        for (int j=0; j<num_Regions; j++)
            expected_value[i][j] = T_j[j] * M_j[j];
}

void SE::vision_selection(int player,int eval){
    // add times of non-investment
    for (int j = 0; j < num_Regions; j++)
        tb[j]++;
    
    // find better region and send to search
    for (int i = 0; i < num_Searchers; i++){
        int j = tool.rand_int(0, num_Regions-1);
        double ev = expected_value[i][j];
        // run player times to find better region
        for (int p = 0; p < num_Players-1; p++){
            int c = tool.rand_int(0, num_Regions-1);
            if (expected_value[i][c] > ev){
                j = c;
                ev = expected_value[i][j];
            }
        }
            
        // assign searcher to region
        searcher_region_id[i] = j;

        // update ta & tb of selected region
        ta[j]++;
        tb[j]=1;
    }
}

void SE::marketing_search(int &Best){
    for (int j = 0; j < num_Regions; j++)
        if (tb[j] > 1)
            ta[j] = 0;
    for (int i = 0; i < num_Searchers; i++){
        if (searcher_sol_fit[i] > Best){
            Best = searcher_sol_fit[i];
            Best_sol = searcher_sol[i];
        }
    }

    for (int j = 0; j < num_Regions; j++)
        for (int k = 0; k < num_Samples; k++)
            if (sample_sol_fit[j][k] > Best){
                Best = sample_sol_fit[j][k];
                Best_sol = sample_sol[j][k];
            }
}

#endif