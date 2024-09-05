#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;
using std::setw;

class AlgPrint{
public:
    AlgPrint(){}
    AlgPrint( int run, const string& folder, const string& alg) : _run(run), _folder(folder), _alg(alg){
        _lastPrint = 0;
    }
    
    void clearResult(const string& filename) {
        ofstream ofs;
        ofs.open(filename, ofstream::out | ofstream::trunc);
        ofs.close();
    }


    template<typename T>
    void NewShowData(vector<vector<T>>& data, int amount){
        data.resize(_run);
        for (int i = 0; i < _run; i++){
            data[i].resize(amount);
        }
        cout << "Data size: " << data.size() << " " << data[0].size() << endl;
    }

    template<typename T>
    void SetData(int run, vector<vector<T>>& data, T num, int iter){
        if (_lastPrint==iter){
            data[run][iter] = num;
            // cout << "Run" << run << " iter: " << iter << " num: " << num << endl;
        }
        else{
            for (int i = _lastPrint+1; i <= iter; i++){
                data[run][i] = num;
                // cout << "Run" << run << " iter: " << i << " num: " << num << endl;
            }
            _lastPrint = iter;
        }
        if (_lastPrint != _maxPrint)
            _maxPrint = _lastPrint;
    }

    template<typename T>
    void PrintToFile(string fileName, vector<vector<T>>& data, int iter){
        // clear old file
        clearResult(fileName);
        // write to file
        ofstream file(fileName, ios_base::app);
        if (file.is_open()) {
            vector<T> AvgData(_maxPrint, 0);

            for (int i = 0; i < _run; i++){
                for (int j = 0; j < _maxPrint; j++){
                    AvgData[j] += data[i][j];
                }
            }
            for(const auto& elem : AvgData){
                file << elem/_run << endl;
            }
        }
        else {
            cerr << "Unable to open file!\n";
        }
    }

    void NewShowDataInt(int amount){
        NewShowData(_dataInt, amount);
    }

    void NewShowDataDouble(int amount){
        NewShowData(_dataDouble, amount);
    }

    void SetDataInt(int run, long int num, int iter){
        SetData(run, _dataInt, num, iter);
    }

    void SetDataDouble(int run, long double num, int iter){
        SetData(run, _dataDouble, num, iter);
    }

    void PrintToFileInt(string fileName, int iter){
        PrintToFile(fileName, _dataInt, iter);
    }

    void PrintToFileDouble(string fileName, int iter){
        PrintToFile(fileName, _dataDouble, iter);
    }


private:
    string _folder;
    string _alg;
    int _run;
    int _lastPrint;
    int _maxPrint;
    vector<vector<long int>> _dataInt;
    vector<vector<long double>> _dataDouble;
};
