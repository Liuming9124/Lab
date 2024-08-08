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
    AlgPrint( int run, const string& folder, const string& alg) : _run(run), _folder(folder), _alg(alg){}
    
    void clearResult(const string& filename) {
        ofstream ofs;
        ofs.open(filename, ofstream::out | ofstream::trunc);
        ofs.close();
    }


    template<typename T>
    void NewShowData(vector<vector<T>>& data, int amount){
        data.resize(_run);
        for (auto& elem : data) {
            elem.resize(amount); 
            std::fill(elem.begin(), elem.end(), T());
        }
    }

    template<typename T>
    void SetData(int run, vector<vector<T>>& data, T num, int iter){
        if (this->_iter < iter){
            cout << data[run].size() << endl;
            cout << "test in" << endl;
            data[run].resize(iter+1);
            for (auto& elem : data[run]){
                std::fill(elem.begin() + _iter, elem.end(), T());
            }
            cout << "test out" << endl;
            this->_iter = iter;
            cout << data[run].size() << endl;
            data[run][iter] = num;
        }
        else {
            // check if index is out of range
            if (run < data.size() && iter < data[run].size()) {
                data[run][iter] += num;
            } else {
                cout << "run: " << run << " iter: " << iter << " num: " << num << endl;
                throw out_of_range("Index out of range");
            }
        }
    }

    template<typename T>
    void PrintToFile(string fileName, vector<vector<T>>& data, int iter){
        // clear old file
        clearResult(fileName);
        // write to file
        ofstream file(fileName, ios_base::app);
        if (file.is_open()) {
            vector<T> AvgData(iter, 0);

            for (int i = 0; i < _run; i++){
                for (int j = 0; j < iter; j++){
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
        _iter = amount;
        NewShowData(_dataInt, amount);
    }

    void NewShowDataDouble(int amount){
        _iter = amount;
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
    int _iter;
    vector<vector<long int>> _dataInt;
    vector<vector<long double>> _dataDouble;
};
