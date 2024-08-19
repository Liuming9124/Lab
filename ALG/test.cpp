#include "Tool.h"
#include <vector>
#include <iostream>
using namespace std;

void asyfunc (double *x)
{
	int i;
    for (i=0; i<10; i++)
    {
        cout << x[i] << endl;
    }
}

int main(){

    // Tool test;
    // for (int i = 0; i < 10; i++){
    //     cout << test.rand_int(0, 10) << endl;
    // }
    vector<double> x;
    x.resize(10);
    for (int i = 0; i < x.size(); i++){
        x[i] = i;
    }
    asyfunc(&x[0]);
}