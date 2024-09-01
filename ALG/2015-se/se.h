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
    int Best_index;
    d1d Best;

    // search algorithm
    i2d searcher_sol;  // [searcher, bit_dim]
    i3d sample_sol;    // [region, sample, bit_dim]
    i2d sample_best;   // [region, bit_dim]
    i4d sampleV_sol;   // [searcher, region, sample, bit_dim]

    d1d searcher_sol_fit;
    d2d sample_sol_fit;
    d1d sample_best_fit;
    d3d sampleV_sol_fit;

    d1d ta; // number of times the j-th region has been invested
    d1d tb; // number of times the j-th region has not been invested

    d2d expected_value; // [i-th investment, j-th region]
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
    void marketing_search(d1d);
    void reset();
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
    show.NewShowDataDouble(num_Fess);

    problem.setStrategy(Func);

    while (num_Run--)
    {
        init();
        resource_arrangement();
        for (int i=0; i<num_Evals; i++)
        {
            vision_search(i);
            marketing_search(Best);
            reset();
        }
    }
    show.PrintToFileDouble("./result/result" + to_string(Func) + ".txt", num_Fess);
    cout << "end" << endl;
}

void SE::init()
{
    searcher_sol.assign(num_Searchers, i1d(num_Dim, 0));
    sample_sol.assign(num_Regions, i2d(num_Samples, i1d(num_Dim, 0)));
    sample_best.assign(num_Regions, i1d(num_Dim, 0));
    sampleV_sol.assign(num_Searchers, i3d(num_Regions, i2d(num_Samples, i1d(num_Dim, 0))));

    searcher_sol_fit.assign(num_Searchers, 0.0);
    sample_sol_fit.assign(num_Regions, d1d(num_Samples, 0.0));
    sample_best_fit.assign(num_Regions, 0.0);
    sampleV_sol_fit.assign(num_Searchers, d2d(num_Regions, d1d(num_Samples, 0.0)));

    Best.assign(num_Dim, 0);
    Best_index = 0;
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
            for (int k=0; k<num_Dim; k++){
                sample_sol[i][j][k] = id_bits[i][k];
            }
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
    for (int i = 0; i < num_Searchers; i++) {
        for (int j = 0; j < num_Regions; j++) {
            for (int k = 0; k < num_Samples; k++) {
                const int x = tool.rand_int(0, num_Dim+1); // divide place, from start to end(must add 1)
                const int m = k << 1; //

                // TODO 這啥
                for (int l = 0; l < num_id_bits; l++) {
                    sampleV_sol[i][j][m][l]   = id_bits[j][l];
                    sampleV_sol[i][j][m+1][l] = id_bits[j][l];
                }
            }
        }
    }
}

void SE::compute_expected_value(int eval){

}

void SE::vision_selection(int ,int ){

}

#endif