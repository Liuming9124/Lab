#include <iostream>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;
using std::setw;
#pragma once


typedef struct City{
    int _x, _y;
} City;

typedef struct Route{
    vector<int> _tour;
    double _length;
} Route;

typedef struct Ant{
    int _currentCity;
    int _nextCity;
    int _numVisited;
    vector<int> _visited;
    vector<int> _allowed;
    Route _path;
} Ant;


class Tsp
{
public:
    double TspProblem(Ant ant, vector<vector<double>> DistanceTable){
        double length = 0.0;
        for (int i = 0; i < ant._numVisited-1; i++){
            length += DistanceTable[ant._path._tour[i]][ant._path._tour[i+1]];
        }
        return length;
    }
};