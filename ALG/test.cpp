#include "Tool.h"
#include <iostream>

int main(){

    Tool test;
    for (int i = 0; i < 10; i++){
        cout << test.rand_int(0, 10) << endl;
    }
}