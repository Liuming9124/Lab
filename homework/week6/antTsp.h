#ifndef ANTTSP_H
#define ANTTSP_H

#include "../problem/TSP.cpp"
#include <vector>
#include <string>
#include <math.h>
#include <sstream>

typedef struct Pheromone{
    double _pheromone;
    double _newPheromone;
} Pheromone;

class AntTsp: Tsp {
public:
    void RunALG(string, int, int, int, int, int, int, double, double);

private:
    int _Run;
    int _iteration;
    int _numCities;
    int _numAnts;
    int _alpha;
    int _beta;
    int _Q;
    double _rho; // evaporation rate
    double _pmute;

    vector<City> _cities;
    vector<vector<double>> _distanceTable;

    vector<Ant> _ants;
    vector<vector<Pheromone>> _pheromoneMatrix;
    vector<double> _probability;
    Route _bestTour, _GlobalBestTour;

    void Init();
    void Evaluation();
    void Reset();

    void ConstructAntSolutions();
    void ApplyLocalSearch();
    void UpdatePheromones();
    
    void Read_City(string);
    void Mutation(Ant &);
    void CheckTwoOpt(Ant &);
    void UpdateProbability();

    double ProbCalculator( Ant &, int, int);
};


void AntTsp::RunALG(string fileName, int Run, int iteration, int numAnts, int alpha, int beta, int rho, double Q, double pmute){
    _Run = Run;
    _iteration = iteration;
    _numAnts = numAnts;
    _alpha = alpha;
    _beta = beta;
    _rho = rho;
    _Q = Q;
    _pmute = pmute;

    Read_City(fileName);

    while (this->_Run--){
        cout << "-------------------Run" << Run - this->_Run << "---------------------" << endl;
        Init();
        Evaluation();
        Reset();
    }
}

void AntTsp::Evaluation(){
    for (int iter = 0; iter < _iteration; iter++){
        for (int i = 0; i < _numCities - 1; i++){
            ConstructAntSolutions();
            ApplyLocalSearch();
            UpdatePheromones();
        }
        // add the last edge
        for (int i = 0; i < _numAnts; i++){
            _ants[i]._path._length += _distanceTable[_ants[i]._path._tour[_numCities-1]][_ants[i]._path._tour[0]];
            _ants[i]._nextCity = _ants[i]._path._tour[0];
            UpdatePheromones();
        }
        // update best tour
        for (int i = 0; i < _numAnts; i++){
            if (iter==0){
                _bestTour = _ants[0]._path;
                continue;
            }
            _bestTour = _bestTour._length > _ants[i]._path._length ? _ants[i]._path : _bestTour;
        }
    }
    for (int i = 0; i < _bestTour._tour.size(); i++){
        cout << _bestTour._tour[i]+1 << " ";
        cout << _cities[_bestTour._tour[i]]._x << " " << _cities[_bestTour._tour[i]]._y << endl;
    }

    cout << "Length: " << _bestTour._length << endl;
}

void AntTsp::Init(){
    // Init Ant and put each ant in a random city
    _ants.resize(_numAnts);
    for (int i = 0; i < _numAnts; i++){
        _ants[i]._path._length = 0.0;
        _ants[i]._path._tour.resize(_numCities+1);
        int place = rand() % _numCities;
        _ants[i]._path._tour[0] = place;
        _ants[i]._currentCity = _ants[i]._path._tour[0];
        _ants[i]._visited.push_back(place);
        _ants[i]._numVisited = 1;
        for (int j = 0; j < _numCities; j++){
            if (j != place){
                _ants[i]._allowed.push_back(j);
            }
        }
    }
    // Init pheromone matrix
    _pheromoneMatrix.resize(_numCities);
    for (int i = 0; i < _numCities; i++){
        _pheromoneMatrix[i].resize(_numCities);
        for (int j = 0; j < _numCities; j++){
            _pheromoneMatrix[i][j]._pheromone = 0.1;
            _pheromoneMatrix[i][j]._newPheromone = 0.0;
        }
    }
}
void AntTsp::Reset(){
    _ants.clear();
    _pheromoneMatrix.clear();
}

void AntTsp::Read_City(string fileName){
    ifstream infile(fileName);
    if (!infile.is_open()) {
        cerr << "Error: Could not open file " << fileName << endl;
        return;
    }
    _numCities = 0;
    string line;
    while (getline(infile, line)) {
        istringstream iss(line);
        int num, x, y;
        if (!(iss >> num >> x >> y)) { 
            cerr << "Error: Incorrect format in line: " << line << endl;
            continue; 
        } // error
        City city;
        city._x = x;
        city._y = y;
        _cities.push_back(city);
        _numCities++;
    }
    infile.close();

    // init distance table
    _distanceTable.resize(_numCities);
    for (int i = 0; i < _numCities; i++){
        _distanceTable[i].resize(_numCities);
    }
    for (int i=0; i < _numCities; i++){
        for (int j=0; j < _numCities; j++){
            _distanceTable[i][j] = sqrt(pow(_cities[i]._x - _cities[j]._x, 2) + pow(_cities[i]._y - _cities[j]._y, 2));
        }
    }
}

void AntTsp::ConstructAntSolutions(){
    // each ant move to the next city based on the probability matrix
    for (int i = 0; i < _numAnts; i++){
        // compute the probability matrix and move to the next city
        if (_ants[i]._allowed.size() == 0){
            continue;
        }
        else{
            _probability.resize(_ants[i]._allowed.size());
            double next = (rand()%1000) / 1000.0;
            for (int j = 0; j < _probability.size(); j++){
                _probability[j] = ProbCalculator( _ants[i], _ants[i]._currentCity, j);
                next -= _probability[j];
                if (next <= 0 || j == _probability.size()-1){
                    _ants[i]._nextCity = _ants[i]._allowed[j];
                    _ants[i]._path._tour[_ants[i]._visited.size()] = _ants[i]._nextCity;
                    _ants[i]._path._length += _distanceTable[_ants[i]._currentCity][_ants[i]._nextCity];
                    _ants[i]._visited.push_back(_ants[i]._allowed[j]);
                    _ants[i]._allowed.erase(_ants[i]._allowed.begin() + j);
                    _ants[i]._numVisited++;
                    break;
                }
            }
            _probability.clear();
        }
    }
}

void AntTsp::ApplyLocalSearch(){
    for (int n = 0; n < _numAnts; n++) {
        _ants[n]._numVisited++;
        _ants[n]._path._tour[_ants[n]._numVisited - 1] = _ants[n]._path._tour[0];

        Mutation(_ants[n]);
        CheckTwoOpt(_ants[n]);
        _ants[n]._path._length = TspProblem(_ants[n], _distanceTable);
        _ants[n]._numVisited--;
    }
}

void AntTsp::UpdatePheromones(){
    // calculate current pheromones
    for (int i = 0; i < _numAnts; i++){
        _pheromoneMatrix[_ants[i]._currentCity][_ants[i]._nextCity]._newPheromone += _Q / _ants[i]._path._length;
        _ants[i]._currentCity = _ants[i]._nextCity;
    }
    // update pheromones
    for (int i = 0; i< _numCities; i++){
        for (int j = 0; j < _numCities; j++){
            _pheromoneMatrix[i][j]._pheromone = (1 - _rho) * _pheromoneMatrix[i][j]._pheromone + _pheromoneMatrix[i][j]._newPheromone;
            _pheromoneMatrix[i][j]._newPheromone = 0.0;
        }
    }
}

void AntTsp::Mutation(Ant &ant){
    double p = (rand()%1000) / 1000.0;
    if (p < _pmute){
        int i = rand() % ant._numVisited;
        int j = rand() % ant._numVisited;
        swap(ant._path._tour[i], ant._path._tour[j]);
    }
}

void AntTsp::CheckTwoOpt(Ant& ant){
    for (int i = 0; i < ant._numVisited - 3; i++) {
        for (int j = i + 2; j < ant._numVisited - 1; j++) { // Adjust the range to avoid out-of-bounds
            if (_distanceTable[ant._path._tour[i]][ant._path._tour[i+1]] + _distanceTable[ant._path._tour[j]][ant._path._tour[j+1]] 
                > _distanceTable[ant._path._tour[i]][ant._path._tour[j]] + _distanceTable[ant._path._tour[i+1]][ant._path._tour[j+1]])
            {
                for (int k=0; k<(j-i)/2; k++){
                    swap(ant._path._tour[i+k+1], ant._path._tour[j-k]);
                }
            }
        }
    }
}

double AntTsp::ProbCalculator(Ant &ant, int start, int dest){
    double Numerator = 0.0; 
    double Denominator = 0.0;
        
    Numerator = pow( _pheromoneMatrix[start][dest]._pheromone, _alpha) * pow(_Q/_distanceTable[start][dest], _beta);
    for (int i = 0; i < ant._allowed.size(); i++){
        Denominator += pow( _pheromoneMatrix[start][ant._allowed[i]]._pheromone, _alpha) * pow(_Q/_distanceTable[start][ant._allowed[i]], _beta);
    }

    return Numerator / Denominator;
}

#endif