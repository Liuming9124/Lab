#include <vector>
#include <cmath>
#include <limits>

using namespace std;

// 計算兩點之間的歐幾里得距離
double euclidean_distance(const vector<double>& a, const vector<double>& b) {
    double distance = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        distance += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return sqrt(distance);
}

// 計算 IGD
double calculate_IGD(const vector<vector<double>>& pareto_front, const vector<vector<double>>& solutions) {
    double total_distance = 0.0;
    int num_pareto_points = pareto_front.size();

    // 對於每個真實 Pareto 前沿上的點
    for (const auto& pareto_point : pareto_front) {
        double min_distance = numeric_limits<double>::infinity();

        // 計算該點到解集合中最近點的距離
        for (const auto& solution : solutions) {
            double distance = euclidean_distance(pareto_point, solution);
            if (distance < min_distance) {
                min_distance = distance;
            }
        }

        // 累加最小距離
        total_distance += min_distance;
    }

    // 返回平均距離
    return total_distance / num_pareto_points;
}