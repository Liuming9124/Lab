#include "../problem/Deception.cpp"

class Tsde: Deception {
public:
    void RunALG(int, int, int, double, double);

private:
    // Input from Command-line Argument
    int _Bit;
    int _Run;
    int _Iter;
    double _Temp;
    double _Rate;

    int nfes;
    int mnfes;

    int _Iter_len = 0;

    std::vector<int> Init();
    void Evaluation(std::vector<int>);
    void Reset(double);

};

void Tsde::RunALG (int Bit, int Run, int Iter, double Temp, double Rate){
    this->_Bit = Bit;
    this->_Run = Run;
    this->_Iter = Iter;
    
    this->_Rate = Rate;
    this->nfes = this->mnfes = 0;

    while (this->_Run--){
        cout << "-------------------Run" << Run - this->_Run << "---------------------" << endl;
        vector<int> sol = Init();
        Evaluation(sol);
        Reset(Temp);
    }
    cout << "Average NFEs : " << this->mnfes/Run << endl;
}