#include <vector>
#include <iostream>

class Deception
{
public:
    std::vector<int> DeceptionProblem(std::vector<int> sol, int bit_size){
        return Abs_Minus(B2D(sol), Power(2, find_highest_bit(sol)-2));
    }
    std::vector<int> GetEndValue(int bit_size){
        std::vector<int> Best(bit_size);
        for (int i = 0; i < bit_size; i++){
            (i==bit_size-2)? Best[i] = 0: Best[i] = 1;
        }
        return Best;
    }
    bool DeceptionProblemCompare(std::vector<int> a, std::vector<int> b, int bit_size){
        std::vector<int> value_a = DeceptionProblem(a, bit_size);
        std::vector<int> value_b = DeceptionProblem(b, bit_size);
        return value_a.size() < value_b.size();
    }
private:
    std::vector<int> B2D(std::vector<int> sol){
        std::vector<int> decimal = {0};
        for (int i = 0; i < sol.size(); i++){ // TODO i 條件 size is moving
            if (sol[i] == 1){
                Adder(&decimal, Power(2, i));
            }
        }
        return decimal;
    }
    std::vector<int> Power(int base, int exp){
        std::vector<int> result = {1};
        for (int i = 0; i < exp; i++){
            for (int x : result){
                x *= base;
            }
            Carryout(&result, i);
        }
        return result;
    }
    void Adder(std::vector<int>* data, std::vector<int> adder){
        for (int i = 0; i < adder.size(); i++){
            (*data)[i] += adder[i];
            Carryout(data, i);
        }
    }
    void Carryout(std::vector<int>* data, int place){
        if ((*data)[place]>=10){
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
    void CarryMinus(std::vector<int>* data, int place){
        if ((*data)[place]<0){
            // carry minus
            int borrow = place+1;
            while ((*data)[borrow]<=0){
                borrow++;
            }
            (*data)[borrow]--;
            for (int i = borrow-1; i > place; i--){
                (*data)[i] += 9;
            }
            (*data)[place] += 10;
        }
    }
    std::vector<int> Abs_Minus(std::vector<int> a, std::vector<int> b){
        if (a.size() < b.size() || (a.size() == b.size() && a[a.size()-1] < b[b.size()-1])){
            std::vector<int> temp;
            temp = a;
            a = b;
            b = temp;
        }
        for (int i = 0; i < a.size(); i++){
            a[i] -= b[i];
            if (a[i] < 0){
                CarryMinus(&a, i);
            }
        }
        return a;
    }
    int find_highest_bit(std::vector<int> sol){
        int highest_bit = 0;
        for (int i = sol.size()-1; i >= 0; i++){
            if (sol[i] == 1){
                highest_bit = i;
                break;
            }
        }
        return highest_bit;
    }
};