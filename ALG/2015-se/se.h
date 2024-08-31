#ifndef SE_H
#define SE_H

#include "../AlgPrint.h"
#include "../Tool.h"
#include <vector>
#include <algorithm>
using namespace std;

class SE
{
public:
    typedef vector<double> d1d;
    typedef vector<d1d> d2d;


public:
    void RunALG(int, int, int, int, int, int, int, int);

private:
    // env
    int _Run;
    int _Fess;
    int _Dim;
    int _Searchers;
    int _Regions;
    int _Samples;
    int _Players;

    // search
    
    
    void init();
    void resource_arrangement();
    void vision_search();
    void marketing_search();
    void reset();
};


void SE::RunALG(int Run, int Problem, int Evals, int Bits, int Searchers, int Regions, int Samples, int Players)
{
    _Run = Run;
    _Fess = Evals;
    _Dim = Bits;
    _Searchers = Searchers;
    _Regions = Regions;
    _Samples = Samples;
    _Players = Players;


    init();
    for (int i = 0; i < _Run; i++){
        resource_arrangement();
        vision_search();
        marketing_search();
        reset();
    }
}


#endif