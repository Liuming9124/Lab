#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;
using std::setw;
#pragma once

class Deception
{
public:
    std::vector<int> DeceptionProblem(std::vector<int> sol){
        int highest_bit = find_highest_bit(sol);
        if (highest_bit > 2){
            std::vector<int> minus ;
            for (int i = 0; i < highest_bit-2; i++){
                minus.push_back(0);
            }
            minus.push_back(1);
            return B2D(Binary_Minus(sol, minus));
        }
        return std::vector<int> {0};
    }
    std::vector<int> GetEndValue(int bit_size){
        std::vector<int> Best(bit_size);
        for (int i = 0; i < bit_size; i++){
            (i==(bit_size-2)-1)? Best[i] = 0: Best[i] = 1;
        }
        return Best;
    }
    bool DeceptionProblemCompare(std::vector<int> a, std::vector<int> b){
        std::vector<int> value_a = DeceptionProblem(a);
        std::vector<int> value_b = DeceptionProblem(b);
        int length;
        value_a.size() > value_b.size()? length = value_a.size(): length = value_b.size();
        for (int i = length-1 ; i >= 0; i--){
            if ( (i >= value_b.size() && value_a[i] == 0) || (i >= value_a.size() && value_b[i] == 0) ){
                continue;
            }
            if (value_a[i] > value_b[i]){
                return true;
            }
            else if (value_a[i] < value_b[i]){
                return false;
            }
            else{
                continue;
            }
        }
        return false;
    }
        
    void Print(int iter, std::vector<int> show, int iter_len, int bit_size, int run){
        cout << "Iter " << std::setw(iter_len) << iter << " : ";
        for (int x : show){
            cout << x;
        }
        cout << ", Value : ";
        for (int x: DeceptionProblem(show)){
            cout << x;
        }
        cout << endl;

        // TODO: Write append without clear
        std::string filename = "../result/Sa/deception/Sa_" + std::to_string(run) + ".txt";
        std::ofstream file(filename, std::ios_base::app);

        if (file.is_open()) {
            file << "Iter " << std::setw(10) << iter << " : ";
            for (int x : show) {
                file << x;
            }
            file << ", Value : ";
            for (int x: DeceptionProblem(show)){
                file << x;
            }
            file << std::endl;
        }
        else {
            std::cerr << "Unable to open file!\n";
        }
    }
    
    void transaction(std::vector<int>* sol, int bit_size){
        int index = rand() % bit_size;
        (*sol)[index] = !(*sol)[index];
    }

private:
    std::vector<int> B2D(std::vector<int> sol){
        std::vector<int> decimal;
        decimal.push_back(0);
        for (int i = 0; i < sol.size(); i++){
            if (sol[i] == 1){
                Adder(&decimal, Power_2(i));
            }
        }
        return decimal;
    }
    std::vector<int> Power_2(int exp){
        std::vector<int> result = {1};
        for (int i = 0; i < exp; i++){
            bool cout_flag = false;
            int cout_start = 0;
            for (int j = 0; j < result.size(); j++){
                result[j] *= 2;
                if (result[j] >= 10 && !cout_flag){
                    cout_flag = true;
                    cout_start = j;
                }
            }
            if (cout_flag){
                Carryout(&result, cout_start);
            }
        }
        return result;
    }
    void Adder(std::vector<int>* data, std::vector<int> adder){
        for (int i = 0; i < adder.size(); i++){
            if (i >= data->size()){
                data->push_back(adder[i]);
            }
            else {
                (*data)[i] += adder[i];
                if ((*data)[i] >= 10){
                    Carryout(data, i);
                }
            }
        }
    }
    void Carryout(std::vector<int>* data, int place){
        if (place >= data->size()) {
            return; // set error break
        }
        else if ((*data)[place]>=10){
            int carryout = (*data)[place]/10;
            (*data)[place] %= 10;
            if (place+1 == data->size()){
                data->push_back(carryout);
            }
            else{
                (*data)[place+1] += carryout;
            }
            Carryout(data, place+1);
        }
    }
    void Binary_CarryMinus(std::vector<int>* data, int place){
        if ((*data)[place]<0){
            int borrow = place+1;
            while ((*data)[borrow]<=0){
                borrow++;
            }
            (*data)[borrow]--;
            for (int i = borrow-1; i >= place; i--){
                (*data)[i] += 1;
            }
        }
    }
    std::vector<int> Binary_Minus(std::vector<int> a, std::vector<int> b){
        if (a.size() < b.size() || (a.size() == b.size() && a[a.size()-1] < b[b.size()-1])){
            std::vector<int> temp;
            temp = a;
            a = b;
            b = temp;
        }
        for (int i = 0; i < b.size(); i++){
            a[i] -= b[i];
            if (a[i] < 0){
                Binary_CarryMinus(&a, i);
            }
        }
        return a;
    }
    int find_highest_bit(std::vector<int> sol){
        int highest_bit = 0;
        for (int i = sol.size()-1; i >= 0; i--){
            if (sol[i] == 1){
                highest_bit = i;
                break;
            }
        }
        return highest_bit;
    }
};