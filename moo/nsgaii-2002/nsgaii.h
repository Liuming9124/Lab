#ifndef NSGAII_H
#define NSGAII_H

#include <vector>
#include <iostream>
#include <algorithm>
#include "../AlgPrint.h"
#include "./problem.cpp"
#include "../Tool.h"

using namespace std;

class NSGAII
{
public:
    typedef struct individual
    {
        int rank;
        double crowdingDistance;
        vector<double> solution;
        double value1, value2;
    } Individual;

    void RunALG(int, int, int, int, int, double, double);

private:
    // env
    int num_Run;
    int num_Evals;
    int num_Dim;
    int num_Population;
    double num_CR;
    double num_MR;

    // variables
    int eval_count;
    vector<Individual> population;

    // Problem
    Problem problem1;
    Problem problem2;

    // Tool
    AlgPrint show;
    Tool tool;
    
    void Init();
    void Evaluation();

    void Crossover(vector<Individual>&);
    void Mutation(vector<Individual>&);
    void Selection();
    
    void FastNonDominatedSort();
    void CrowdingDistanceAssignment();

    // void CreateOffspring();
    void GetFitness(Individual& ind);
};

void NSGAII::RunALG(int Run, int Func, int Evals, int Dim, int Population, double CR, double MR)
{
    num_Run = Run;
    num_Evals = Evals;
    num_Dim = Dim;
    num_Population = Population;
    num_CR = CR;
    num_MR = MR;

    if (Func == 1) {
        problem1.setStrategy(1);
        problem2.setStrategy(2);
    }

    show = AlgPrint(num_Run, "./result", "nsgaii");

    while (num_Run--)
    {
        cout << "-------------------Run" << Run - num_Run << "---------------------" << endl;
        Init();
        while (eval_count < num_Evals)
            Evaluation();
    }
    // show data
}

void NSGAII::Init() {
    eval_count = 0;
    population.resize(num_Population);
    for (int i =0; i<num_Population; i++) {
        population[i].solution.resize(num_Dim);
        for (int j=0; j<num_Dim; j++) {
            population[i].solution[j] = tool.rand_double(problem1.getBounderMin(), problem1.getBounderMax());
        }
        population[i].crowdingDistance = 0.0;
        population[i].rank = -1;
        GetFitness(population[i]);
    }
}

void NSGAII::Evaluation() {
    // Step 1: 計算當前種群的目標值
    for (Individual& ind : population) {
        GetFitness(ind); // 計算 ind 的 value1 和 value2
    }

    // Step 2: 非支配排序
    FastNonDominatedSort();

    // Step 3: 計算擁擠距離
    CrowdingDistanceAssignment();

    // Step 4: 生成子代種群
    vector<Individual> offspring;
    Crossover(offspring);
    Mutation(offspring);

    // Step 5: 計算子代的目標值
    for (Individual& ind : offspring) {
        GetFitness(ind);
    }


    // Step 6: 合併父代與子代
    population.insert(population.end(), offspring.begin(), offspring.end());

    // Step 7: 選擇新種群
    Selection();
}


void NSGAII::FastNonDominatedSort() {
    vector<vector<int>> fronts; // fronts[i] 表示第 i 層的個體索引
    fronts.push_back(vector<int>()); // 初始化第一層前沿

    vector<int> dominationCount(num_Population, 0); // 被支配的次數
    vector<vector<int>> dominatedSet(num_Population); // 支配其他個體的集合

    // 計算支配關係
    for (int p = 0; p < num_Population; p++) {
        for (int q = 0; q < num_Population; q++) {
            if (p == q) continue;

            bool dominatesPtoQ = 
                (population[p].value1 <= population[q].value1 && population[p].value2 <= population[q].value2) &&
                (population[p].value1 < population[q].value1 || population[p].value2 < population[q].value2);

            bool dominatesQtoP = 
                (population[q].value1 <= population[p].value1 && population[q].value2 <= population[p].value2) &&
                (population[q].value1 < population[p].value1 || population[q].value2 < population[p].value2);

            if (dominatesPtoQ) {
                dominatedSet[p].push_back(q); // p 支配 q
            } else if (dominatesQtoP) {
                dominationCount[p]++; // p 被 q 支配
            }
        }

        // 如果 p 沒有被任何個體支配，則屬於第一層
        if (dominationCount[p] == 0) {
            population[p].rank = 1; // 設定為第一層
            fronts[0].push_back(p);
        }
    }

    // 建立其他層的前沿
    int currentRank = 1;
    while (!fronts[currentRank - 1].empty()) {
        vector<int> nextFront; // 下一層的個體
        for (int p : fronts[currentRank - 1]) {
            for (int q : dominatedSet[p]) {
                dominationCount[q]--; // 減少 q 被支配的次數
                if (dominationCount[q] == 0) {
                    population[q].rank = currentRank + 1; // 屬於下一層
                    nextFront.push_back(q);
                }
            }
        }
        fronts.push_back(nextFront);
        currentRank++;
    }
}

void NSGAII::CrowdingDistanceAssignment() {
    // 初始化所有個體的擁擠距離
    for (Individual& ind : population) {
        ind.crowdingDistance = 0.0;
    }

    // 將個體根據 rank 分成不同的前沿層處理
    vector<vector<Individual*>> fronts;
    for (Individual& ind : population) {
        if (ind.rank > fronts.size()) {
            fronts.resize(ind.rank); // 動態增加層數
        }
        fronts[ind.rank - 1].push_back(&ind);
    }

    // 計算每個前沿層的擁擠距離
    for (auto& front : fronts) {
        if (front.size() <= 2) {
            // 如果層內只有 1 或 2 個個體，直接設定無窮大擁擠距離
            for (Individual* ind : front) {
                ind->crowdingDistance = std::numeric_limits<double>::infinity();
            }
            continue;
        }

        // 對每個目標分別進行排序和擁擠距離計算
        for (int m = 0; m < 2; m++) { // 假設有兩個目標
            // 按目標 m 的值進行排序
            sort(front.begin(), front.end(), [m](Individual* a, Individual* b) {
                return (m == 0) ? (a->value1 < b->value1) : (a->value2 < b->value2);
            });

            // 邊界解的擁擠距離設置為無窮大
            front[0]->crowdingDistance = std::numeric_limits<double>::infinity();
            front.back()->crowdingDistance = std::numeric_limits<double>::infinity();

            // 計算內部個體的擁擠距離
            for (size_t i = 1; i < front.size() - 1; i++) {
                double diff = (m == 0)
                    ? (front[i + 1]->value1 - front[i - 1]->value1)
                    : (front[i + 1]->value2 - front[i - 1]->value2);

                double range = (m == 0)
                    ? (front.back()->value1 - front[0]->value1)
                    : (front.back()->value2 - front[0]->value2);

                if (range > 0) {
                    front[i]->crowdingDistance += diff / range;
                }
            }
        }
    }
}

void NSGAII::Crossover(vector<Individual>& offspring) {
    for (size_t i = 0; i < num_Population / 2; i++) {
        int p1_idx = tool.rand_int(0, num_Population - 1);
        int p2_idx = tool.rand_int(0, num_Population - 1);
        Individual parent1 = population[p1_idx];
        Individual parent2 = population[p2_idx];

        Individual child1 = parent1;
        Individual child2 = parent2;

        for (int j = 0; j < num_Dim; j++) {
            if (tool.rand_double(0.0, 1.0) <= num_CR) {
                double beta;
                double u = tool.rand_double(0.0, 1.0);
                if (u <= 0.5) {
                    beta = pow(2.0 * u, 1.0 / (num_CR + 1));
                } else {
                    beta = pow(1.0 / (2.0 * (1.0 - u)), 1.0 / (num_CR + 1));
                }
                child1.solution[j] = 0.5 * ((1 + beta) * parent1.solution[j] + (1 - beta) * parent2.solution[j]);
                child2.solution[j] = 0.5 * ((1 - beta) * parent1.solution[j] + (1 + beta) * parent2.solution[j]);
            }
        }
        offspring.push_back(child1);
        offspring.push_back(child2);
    }
}

void NSGAII::Mutation(vector<Individual>& offspring) {
    for (Individual& ind : offspring) {
        for (int j = 0; j < num_Dim; j++) {
            if (tool.rand_double(0.0, 1.0) <= num_MR) {
                double u = tool.rand_double(0.0, 1.0);
                double delta;
                if (u <= 0.5) {
                    delta = pow(2.0 * u, 1.0 / (num_MR + 1)) - 1.0;
                } else {
                    delta = 1.0 - pow(2.0 * (1.0 - u), 1.0 / (num_MR + 1));
                }
                ind.solution[j] += delta * (problem1.getBounderMax() - problem1.getBounderMin());
                ind.solution[j] = max(problem1.getBounderMin(), min(problem1.getBounderMax(), ind.solution[j]));
            }
        }
    }
}

void NSGAII::Selection() {
    // Step 1: 非支配排序
    vector<vector<Individual*>> fronts; // 用於存放各層前沿
    vector<Individual*> combinedPopulationPtrs; // 指針方便操作
    for (Individual& ind : population) {
        combinedPopulationPtrs.push_back(&ind);
    }
    // 非支配排序，將個體劃分到不同的前沿
    for (Individual* ind : combinedPopulationPtrs) {
        if (ind->rank > fronts.size()) {
            fronts.resize(ind->rank);
        }
        fronts[ind->rank - 1].push_back(ind);
    }
    // Step 2: 選擇新種群
    vector<Individual> newPopulation; // 用於存放選擇的新種群
    int currentSize = 0;

    for (auto& front : fronts) {
        if (currentSize + front.size() <= num_Population) {
            // 如果當前前沿的個體可以完全加入新種群
            for (Individual* ind : front) {
                newPopulation.push_back(*ind);
            }
            currentSize += front.size();
        } else {
            // 如果當前前沿需要裁剪
            sort(front.begin(), front.end(), [](Individual* a, Individual* b) {
                return a->crowdingDistance > b->crowdingDistance; // 擁擠距離由大到小排序
            });

            for (size_t i = 0; i < num_Population - currentSize; i++) {
                newPopulation.push_back(*front[i]);
            }
            break; // 種群已滿，退出
        }
    }
    // 更新種群
    population = newPopulation;
}

void NSGAII::GetFitness(Individual& ind) {
    ind.value1 = problem1.executeStrategy(ind.solution, num_Dim);
    ind.value2 = problem2.executeStrategy(ind.solution, num_Dim);
    eval_count += 2;
}


#endif