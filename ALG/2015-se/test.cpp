#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>

using namespace std;

typedef vector<int> i1d;
typedef vector<i1d> i2d;
typedef vector<i2d> i3d;
typedef vector<i3d> i4d;
typedef vector<double> d1d;
typedef vector<d1d> d2d;
typedef vector<d2d> d3d;

int main(){
    
    i2d id_bits;
    id_bits.assign(10, i1d(3, 0));
    for (int i=0; i<10; i++)
    {
        int n = i;
        int j = 3;
        while (n > 0) {
            id_bits[i][--j] = n%2;
            n /= 2;
        }
    }
    for (int i=0; i<10; i++)
    {
        for (int j=0; j<3; j++)
            cout << id_bits[i][j] << " ";
        cout << endl;
    }
//     // assign searchers to regions
//     for (int i=0; i<num_Searchers; i++)
//     {
//         // assign region id to each searcher
//         const int r = tool.rand_int(0, num_Regions-1);
//         searcher_region_id[i] = r;

//         for (int j=0; j < num_id_bits; j++)
//             searcher_sol[i][j] = id_bits[r][j];
//     }
}