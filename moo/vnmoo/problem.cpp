#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <string>

#define PI 3.14159265358979323846

using namespace std;

// Abstract Strategy Class
class Strategy {
public:
    virtual vector<double> execute(const vector<double>& population) const = 0;
    virtual vector<double> getBounderMin() const = 0;
    virtual vector<double> getBounderMax() const = 0;
    virtual ~Strategy() = default;
};

// Concrete Strategy for UF Problems
class UFProblemStrategy : public Strategy {
public:
    UFProblemStrategy(const string& funcName) : functionName(funcName) {
        initializeBounds();
    }

    vector<double> execute(const vector<double>& solution) const override {
        if (functionName == "UF1")  return UF1(solution);
        if (functionName == "UF2")  return UF2(solution);
        if (functionName == "UF3")  return UF3(solution);
        if (functionName == "UF4")  return UF4(solution);
        if (functionName == "UF5")  return UF5(solution);
        if (functionName == "UF6")  return UF6(solution);
        if (functionName == "UF7")  return UF7(solution);
        if (functionName == "UF8")  return UF8(solution);
        if (functionName == "UF9")  return UF9(solution);
        if (functionName == "UF10") return UF10(solution);
        return {};
    }

    vector<double> getBounderMin() const override {
        return lb;
    }

    vector<double> getBounderMax() const override {
        return ub;
    }

    int getFunctionDimension() const {
        if (functionName == "UF1" || functionName == "UF2" || functionName == "UF3" || functionName == "UF4" || functionName == "UF5" || 
            functionName == "UF6" || functionName == "UF7" || functionName == "UF8" || functionName == "UF9" || functionName == "UF10") {
            return 30; // 固定為30維度
        } else {
            return -1; // 無效的函數名稱
        }
    }

    int getResultCount() const {
        if (functionName == "UF1" || functionName == "UF2" || functionName == "UF3" || 
            functionName == "UF4" || functionName == "UF5" || functionName == "UF6" || 
            functionName == "UF7") {
            return 2; // 這些函數返回 2 個結果
        } else if (functionName == "UF8" || functionName == "UF9" || functionName == "UF10") {
            return 3; // 這些函數返回 3 個結果
        } else {
            cerr << "Error: Invalid function name!" << endl;
            return -1; // 無效函數名稱
        }
    }

private:
    string functionName;
    vector<double> lb;
    vector<double> ub;

    void initializeBounds() {
        int dimension = 30;
        lb.resize(dimension);
        ub.resize(dimension);

        if (functionName == "UF1") {
            lb[0] = 0.0; ub[0] = 1.0;
            for (int i = 1; i < dimension; i++) {
                lb[i] = -1.0; ub[i] = 1.0;
            }
        } else if (functionName == "UF2" || functionName == "UF3" || functionName == "UF4") {
            lb[0] = 0.0; ub[0] = 1.0;
            for (int i = 1; i < dimension; i++) {
                lb[i] = -1.0; ub[i] = 1.0;
            }
        } else if (functionName == "UF5" || functionName == "UF6" || functionName == "UF7") {
            lb[0] = 0.0; ub[0] = 1.0;
            for (int i = 1; i < dimension; i++) {
                lb[i] = -1.0; ub[i] = 1.0;
            }
        } else if (functionName == "UF8" || functionName == "UF9" || functionName == "UF10") {
            lb[0] = lb[1] = 0.0; ub[0] = ub[1] = 1.0;
            for (int i = 2; i < dimension; i++) {
                lb[i] = -2.0; ub[i] = 2.0;
            }
        }
    }

    // Define UF1 to UF10 functions
    vector<double> UF1(const vector<double>& solution) const {
        int dimension = 30;
        vector<double> result(2, 0.0);

        double sum1 = 0.0, sum2 = 0.0;
        for (int j = 2; j <= dimension; j++) {
            double yj = solution[j - 1] - sin(6.0 * PI * solution[0] + j * PI / dimension);
            if (j % 2 == 0) {
                sum2 += yj * yj;
            } else {
                sum1 += yj * yj;
            }
        }
        result[0] = solution[0] + 2.0 * sum1 / (dimension / 2);
        result[1] = 1.0 - sqrt(solution[0]) + 2.0 * sum2 / (dimension / 2);

        return result;
    }

    vector<double> UF2(const vector<double>& solution) const {
        int dimension = solution.size();
        vector<double> result(2, 0.0); // 每個 solution 有兩個結果

        double sum1 = 0.0, sum2 = 0.0;
        for (int j = 2; j <= dimension; ++j) {
            double yj = solution[j - 1] - 0.3 * solution[0] *
                        (solution[0] * cos(24.0 * PI * solution[0] + 4.0 * j * PI / dimension) + 2.0) *
                        cos(6.0 * PI * solution[0] + j * PI / dimension);
            if (j % 2 == 0) {
                sum2 += yj * yj;
            } else {
                sum1 += yj * yj;
            }
        }

        result[0] = solution[0] + 2.0 * sum1 / (dimension / 2);
        result[1] = 1.0 - sqrt(solution[0]) + 2.0 * sum2 / (dimension / 2);

        return result;
    }

    vector<double> UF3(const vector<double>& solution) const {
        int dimension = solution.size();
        vector<double> result(2, 0.0); // 每個 solution 有兩個結果

        double sum1 = 0.0, sum2 = 0.0, prod1 = 1.0, prod2 = 1.0;
        for (int j = 2; j <= dimension; ++j) {
            double yj = solution[j - 1] - pow(solution[0],
                        0.5 * (1.0 + 3.0 * (j - 2.0) / (dimension - 2.0)));
            double pj = cos(20.0 * yj * PI / sqrt(j + 0.0));
            if (j % 2 == 0) {
                sum2 += yj * yj;
                prod2 *= pj;
            } else {
                sum1 += yj * yj;
                prod1 *= pj;
            }
        }

        result[0] = solution[0] + 2.0 * (4.0 * sum1 - 2.0 * prod1 + 2.0) / (dimension / 2);
        result[1] = 1.0 - sqrt(solution[0]) + 2.0 * (4.0 * sum2 - 2.0 * prod2 + 2.0) / (dimension / 2);

        return result;
    }

    vector<double> UF4(const vector<double>& solution) const {
        int dimension = solution.size();
        vector<double> result(2, 0.0); // 每個 solution 有兩個結果

        double sum1 = 0.0, sum2 = 0.0;
        for (int j = 2; j <= dimension; ++j) {
            double yj = solution[j - 1] - sin(6.0 * PI * solution[0] + j * PI / dimension);
            double hj = fabs(yj) / (1.0 + exp(2.0 * fabs(yj)));
            if (j % 2 == 0) {
                sum2 += hj;
            } else {
                sum1 += hj;
            }
        }

        result[0] = solution[0] + 2.0 * sum1 / (dimension / 2);
        result[1] = 1.0 - pow(solution[0], 2.0) + 2.0 * sum2 / (dimension / 2);

        return result;
    }

    vector<double> UF5(const vector<double>& solution) const {
        int dimension = solution.size();
        vector<double> result(2, 0.0); // 每個 solution 有兩個結果

        double sum1 = 0.0, sum2 = 0.0;
        for (int j = 2; j <= dimension; ++j) {
            double yj = solution[j - 1] - sin(6.0 * PI * solution[0] + j * PI / dimension);
            double hj = 2.0 * yj * yj - cos(4.0 * PI * yj) + 1.0;
            if (j % 2 == 0) {
                sum2 += hj;
            } else {
                sum1 += hj;
            }
        }

        result[0] = solution[0] + 2.0 * sum1 / (dimension / 2);
        result[1] = 1.0 - solution[0] + 2.0 * sum2 / (dimension / 2);

        return result;
    }

    vector<double> UF6(const vector<double>& solution) const {
        int dimension = solution.size();
        vector<double> result(2, 0.0); // 每個 solution 有兩個結果

        double sum1 = 0.0, sum2 = 0.0;
        for (int j = 2; j <= dimension; ++j) {
            double yj = solution[j - 1] - sin(6.0 * PI * solution[0] + j * PI / dimension);
            if (j % 2 == 0) {
                sum2 += yj * yj;
            } else {
                sum1 += yj * yj;
            }
        }

        result[0] = solution[0] + 2.0 * sum1 / (dimension / 2);
        result[1] = 1.0 - solution[0] + 2.0 * sum2 / (dimension / 2);

        return result;
    }

    vector<double> UF7(const vector<double>& solution) const {
        int dimension = solution.size();
        vector<double> result(2, 0.0); // 每個 solution 有兩個結果

        double sum1 = 0.0, sum2 = 0.0;
        for (int j = 2; j <= dimension; ++j) {
            double yj = solution[j - 1] - sin(6.0 * PI * solution[0] + j * PI / dimension);
            if (j % 2 == 0) {
                sum2 += yj * yj;
            } else {
                sum1 += yj * yj;
            }
        }

        result[0] = pow(solution[0], 0.2) + 2.0 * sum1 / (dimension / 2);
        result[1] = 1.0 - pow(solution[0], 0.2) + 2.0 * sum2 / (dimension / 2);

        return result;
    }

    vector<double> UF8(const vector<double>& solution) const {
        int dimension = solution.size();
        vector<double> result(3, 0.0); // 每個 solution 有三個結果

        double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
        int count1 = 0, count2 = 0, count3 = 0;

        for (int j = 3; j <= dimension; ++j) {
            double yj = solution[j - 1] - 2.0 * solution[1] * sin(2.0 * PI * solution[0] + j * PI / dimension);
            if (j % 3 == 1) {
                sum1 += yj * yj;
                count1++;
            } else if (j % 3 == 2) {
                sum2 += yj * yj;
                count2++;
            } else {
                sum3 += yj * yj;
                count3++;
            }
        }

        result[0] = cos(0.5 * PI * solution[0]) * cos(0.5 * PI * solution[1]) + 2.0 * sum1 / count1;
        result[1] = cos(0.5 * PI * solution[0]) * sin(0.5 * PI * solution[1]) + 2.0 * sum2 / count2;
        result[2] = sin(0.5 * PI * solution[0]) + 2.0 * sum3 / count3;

        return result;
    }

    vector<double> UF9(const vector<double>& solution) const {
        int dimension = solution.size();
        double E = 0.1;
        vector<double> result(3, 0.0); // 每個 solution 有三個結果

        double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
        int count1 = 0, count2 = 0, count3 = 0;

        for (int j = 3; j <= dimension; ++j) {
            double yj = solution[j - 1] - 2.0 * solution[1] * sin(2.0 * PI * solution[0] + j * PI / dimension);
            if (j % 3 == 1) {
                sum1 += yj * yj;
                count1++;
            } else if (j % 3 == 2) {
                sum2 += yj * yj;
                count2++;
            } else {
                sum3 += yj * yj;
                count3++;
            }
        }

        double hj = (1.0 + E) * (1.0 - 4.0 * (solution[0] - 0.5) * (solution[0] - 0.5));
        hj = max(hj, 0.0); // Ensure non-negative

        result[0] = 0.5 * hj * solution[1] + 2.0 * sum1 / count1;
        result[1] = 0.5 * (1.0 - hj) * solution[1] + 2.0 * sum2 / count2;
        result[2] = 1.0 - solution[1] + 2.0 * sum3 / count3;

        return result;
    }

    vector<double> UF10(const vector<double>& solution) const {
        int dimension = solution.size();
        vector<double> result(3, 0.0); // 每個 solution 有三個結果

        double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
        int count1 = 0, count2 = 0, count3 = 0;

        for (int j = 3; j <= dimension; ++j) {
            double yj = solution[j - 1] - 2.0 * solution[1] * sin(2.0 * PI * solution[0] + j * PI / dimension);
            double hj = 4.0 * yj * yj - cos(8.0 * PI * yj) + 1.0;

            if (j % 3 == 1) {
                sum1 += hj;
                count1++;
            } else if (j % 3 == 2) {
                sum2 += hj;
                count2++;
            } else {
                sum3 += hj;
                count3++;
            }
        }

        result[0] = cos(0.5 * PI * solution[0]) * cos(0.5 * PI * solution[1]) + 2.0 * sum1 / count1;
        result[1] = cos(0.5 * PI * solution[0]) * sin(0.5 * PI * solution[1]) + 2.0 * sum2 / count2;
        result[2] = sin(0.5 * PI * solution[0]) + 2.0 * sum3 / count3;

        return result;
    }

};

// Problem Class
class Problem {
public:
    void setStrategy(const string& funcName) {
        strategy = make_unique<UFProblemStrategy>(funcName);
    }

    vector<double> executeStrategy(const vector<double>& solution) const {
        if (strategy) {
            return strategy->execute(solution);
        } else {
            cerr << "Error: Strategy not set!" << endl;
            return {};
        }
    }

    vector<double> getBounderMin() const {
        return strategy ? strategy->getBounderMin() : vector<double>();
    }

    vector<double> getBounderMax() const {
        return strategy ? strategy->getBounderMax() : vector<double>();
    }

    int getFunctionDimension() const {
        if (strategy) {
            // 使用動態轉換確保能調用 UFProblemStrategy 的方法
            auto ufStrategy = dynamic_cast<UFProblemStrategy*>(strategy.get());
            if (ufStrategy) {
                return ufStrategy->getFunctionDimension();
            } else {
                cerr << "Error: Strategy is not a UFProblemStrategy!" << endl;
                return -1; // 表示錯誤
            }
        } else {
            cerr << "Error: Strategy not set!" << endl;
            return -1; // 表示錯誤
        }
    }

    int getResultCount() const {
        if (strategy) {
            // 使用動態轉換確保能調用 UFProblemStrategy 的方法
            auto ufStrategy = dynamic_cast<UFProblemStrategy*>(strategy.get());
            if (ufStrategy) {
                return ufStrategy->getResultCount();
            } else {
                cerr << "Error: Strategy is not a UFProblemStrategy!" << endl;
                return -1; // 表示錯誤
            }
        } else {
            cerr << "Error: Strategy not set!" << endl;
            return -1; // 表示錯誤
        }
    }


private:
    unique_ptr<Strategy> strategy;
};

// Main Function for Testing
// int main() {
//     Problem problem;

//     // Set the strategy to a specific UF problem
//     string funcName = "UF4"; // Adjust as needed
//     problem.setStrategy(funcName);

//     // Define a sample population
//     vector<vector<double>> population = {
//         {0.5, 0.3, 0.7, 0.1, 0.2, 0.5, 0.4, 0.8, 0.6, 0.3}, // Example with 10 dimensions
//         {0.1, 0.9, 0.2, 0.8, 0.4, 0.7, 0.2, 0.3, 0.6, 0.5},
//         {0.6, 0.5, 0.3, 0.7, 0.2, 0.1, 0.9, 0.4, 0.2, 0.8}
//     };

//     // Execute the strategy and get fitness results
//     auto results = problem.executeStrategy(population);

//     cout << "Fitness Results for UF4:" << endl;
//     for (const auto& res : results) {
//         for (double val : res) {
//             cout << val << " ";
//         }
//         cout << endl;
//     }

//     // Get bounds for the selected problem
//     auto lowerBounds = problem.getLowerBounds();
//     auto upperBounds = problem.getUpperBounds();

//     cout << "Lower Bounds: ";
//     for (const auto& lb : lowerBounds) {
//         cout << lb << " ";
//     }
//     cout << endl;

//     cout << "Upper Bounds: ";
//     for (const auto& ub : upperBounds) {
//         cout << ub << " ";
//     }
//     cout << endl;

//     return 0;
// }
