#include<iostream>
#include<fstream>
using namespace std;

int main()
{
    double Temperture = 100;
    double cool_down = 90;//0.9

    fstream f;
    // f.open("temperture_in_iteration.txt", ios::out);
    f.open("T_" + to_string((int)Temperture) + "CD_" + to_string((int)cool_down) + ".txt", ios::out);

    for(int iteration = 0; iteration < 100; iteration++)
    {
        f << Temperture << " ";
        f << endl;
        Temperture = Temperture  *  (cool_down / 100);
    }

    f.close();
    return 0;
}