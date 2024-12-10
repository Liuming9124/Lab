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
    cout << "Pareto Front (Final Population):" << endl;
    for (const auto& ind : population) {
        if (ind.rank == 1) { // 只輸出第一層 Pareto 前沿
            cout << "f1: " << ind.value1 << ", f2: " << ind.value2 << endl;
        }
    }
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
    FastNonDominatedSort();
    CrowdingDistanceAssignment();

    // 生成子代種群
    vector<Individual> offspring;
    Crossover(offspring);
    Mutation(offspring);

    // 計算子代的目標值
    for (Individual& ind : offspring) {
        GetFitness(ind);
    }
    // 合併父代與子代
    population.insert(population.end(), offspring.begin(), offspring.end());
    FastNonDominatedSort();
    // 選擇新種群
    Selection();
}

void NSGAII::FastNonDominatedSort() {
    vector<vector<int>> fronts; // store different fronts
    fronts.push_back(vector<int>()); // init first front

    int populationSize = population.size();
    vector<int> dominationCount(populationSize, 0); // 初始化每個個體被支配的次數為 0
    vector<vector<int>> dominatedSet(populationSize); // 初始化每個個體支配的集合

    // 遍歷所有個體，計算支配關係
    for (int p = 0; p < populationSize; p++) {
        for (int q = 0; q < populationSize; q++) {
            if (p == q) continue; // 跳過自己

            // 判斷p是否支配q
            bool dominatesPtoQ = 
                (population[p].value1 <= population[q].value1 && population[p].value2 <= population[q].value2) &&
                (population[p].value1 < population[q].value1 || population[p].value2 < population[q].value2);

            // 判斷q是否支配p
            bool dominatesQtoP = 
                (population[q].value1 <= population[p].value1 && population[q].value2 <= population[p].value2) &&
                (population[q].value1 < population[p].value1 || population[q].value2 < population[p].value2);

            // 更新支配集合和支配次數
            if (dominatesPtoQ)
                dominatedSet[p].push_back(q);
            else if (dominatesQtoP)
                dominationCount[p]++;
        }

        // 如果個體 p 不被任何個體支配，則將其劃分到第一層 Pareto 前沿
        if (dominationCount[p] == 0) {
            population[p].rank = 1;
            fronts[0].push_back(p);
        }
    }

    // 生成其他 Pareto 前沿
    int currentRank = 1; // 當前 Pareto 前沿的等級
    while (!fronts[currentRank - 1].empty()) {
        vector<int> nextFront;  // 下一層 Pareto 前沿的個體集合
        for (int p : fronts[currentRank - 1]) { // 遍歷當前 Pareto 前沿的每個個體
            for (int q : dominatedSet[p]) { // 找到被這些個體支配的其他個體
                dominationCount[q]--; // 被支配次數減一
                if (dominationCount[q] == 0) { // 如果某個個體不再被其他個體支配
                    population[q].rank = currentRank + 1; // 設置 rank 為下一層 Pareto 前沿
                    nextFront.push_back(q); // 加入下一層 Pareto 前沿
                }
            }
        }
        fronts.push_back(nextFront);    // 更新下一層 Pareto 前沿
        currentRank++;  // 遞增 Pareto 前沿等級
    }
}


void NSGAII::CrowdingDistanceAssignment() {
    // 初始化擁擠距離為0
    for (Individual& ind : population) {
        ind.crowdingDistance = 0.0;
    }

    // 依照 rank 對個體劃分到不同的非支配層
    vector<vector<Individual*>> fronts; // 存放不同非支配層的集合
    for (Individual& ind : population) {
        if (ind.rank > fronts.size()) {
            fronts.resize(ind.rank); // 動態調整層數
        }
        fronts[ind.rank - 1].push_back(&ind); // 將個體指針加入對應層
    }

    // 對每一層計算 crowding distance
    for (auto& front : fronts) {
        // 如果層中個體數量小於等於 2，直接設置擁擠距離為無窮大
        if (front.size() <= 2) {
            for (Individual* ind : front) {
                ind->crowdingDistance = std::numeric_limits<double>::infinity();
            }
            continue;
        }

        // 對每個目標進行排序，分別計算擁擠距離
        for (int m = 0; m < 2; m++) {
            // 根據 fitness value 大小進行 increasing sort
            sort(front.begin(), front.end(), [m](Individual* a, Individual* b) {
                return (m == 0) ? (a->value1 < b->value1) : (a->value2 < b->value2);
            });

            // 針對 normalize fitness 找出範圍
            double range = (m == 0) 
                ? (front.back()->value1 - front[0]->value1) 
                : (front.back()->value2 - front[0]->value2);

            if (range == 0) range = 1e-6; // 避免除零錯誤

            // 設置邊界的擁擠距離為無窮大
            front[0]->crowdingDistance = std::numeric_limits<double>::infinity();
            front.back()->crowdingDistance = std::numeric_limits<double>::infinity();

            // 計算其他個體的擁擠距離
            for (size_t i = 1; i < front.size() - 1; i++) {
                double diff = (m == 0) 
                    ? (front[i + 1]->value1 - front[i - 1]->value1) 
                    : (front[i + 1]->value2 - front[i - 1]->value2);
                // normalization
                front[i]->crowdingDistance += diff / range;
            }
        }
    }
}


void NSGAII::Crossover(vector<Individual>& offspring) {
    // 每次生成兩個子代，總共執行種群大小的一半次數
    for (size_t i = 0; i < num_Population / 2; i++) {
        // 隨機選擇兩個父代
        int p1_idx = tool.rand_int(0, num_Population - 1);
        int p2_idx = tool.rand_int(0, num_Population - 1);
        Individual parent1 = population[p1_idx];
        Individual parent2 = population[p2_idx];

        // 初始化子代
        Individual child1 = parent1;
        Individual child2 = parent2;

        // 對每個維度進行 Crossover
        for (int j = 0; j < num_Dim; j++) {
            if (tool.rand_double(0.0, 1.0) <= num_CR) {
                double beta; // 模擬二進制交叉的參數
                double u = tool.rand_double(0.0, 1.0);
                if (u <= 0.5)
                    beta = pow(2.0 * u, 1.0 / (num_CR + 1));
                else
                    beta = pow(1.0 / (2.0 * (1.0 - u)), 1.0 / (num_CR + 1));

                child1.solution[j] = 0.5 * ((1 + beta) * parent1.solution[j] + (1 - beta) * parent2.solution[j]);
                child2.solution[j] = 0.5 * ((1 - beta) * parent1.solution[j] + (1 + beta) * parent2.solution[j]);
            }
            // Check boundary
            child1.solution[j] = max(problem1.getBounderMin(), min(problem1.getBounderMax(), child1.solution[j]));
            child2.solution[j] = max(problem1.getBounderMin(), min(problem1.getBounderMax(), child2.solution[j]));
        }
        // Add to offspring
        offspring.push_back(child1);
        offspring.push_back(child2);
    }
}

void NSGAII::Mutation(vector<Individual>& offspring) {
    // 對每個子代進行突變，挑選一個維度進行突變
    for (Individual& ind : offspring) {
        for (int j = 0; j < num_Dim; j++) {
            if (tool.rand_double(0.0, 1.0) <= num_MR) {
                double u = tool.rand_double(0.0, 1.0);
                double delta; // 控制 Mutation 參數
                if (u <= 0.5)
                    delta = pow(2.0 * u, 1.0 / (num_MR + 1)) - 1.0;
                else
                    delta = 1.0 - pow(2.0 * (1.0 - u), 1.0 / (num_MR + 1));

                // Mutation
                ind.solution[j] += delta * (problem1.getBounderMax() - problem1.getBounderMin());
                // Check boundary
                ind.solution[j] = max(problem1.getBounderMin(), min(problem1.getBounderMax(), ind.solution[j]));
            }
        }
    }
}

void NSGAII::Selection() {
    // 非支配排序 : 依照 rank 將個體劃分到不同的前沿
    vector<vector<Individual*>> fronts; // 用於存放各層前沿
    vector<Individual*> combinedPopulationPtrs; // 指針方便操作
    for (Individual& ind : population) {
        combinedPopulationPtrs.push_back(&ind);
    }
    // 非支配排序，將個體劃分到不同的前沿
    for (Individual* ind : combinedPopulationPtrs) {
        if (ind->rank > fronts.size()) {
            fronts.resize(ind->rank); // 根據 rank 動態調整層數
        }
        fronts[ind->rank - 1].push_back(ind); // 按 rank 分配到對應的層
    }
    // 用於存放新的種群
    vector<Individual> newPopulation; // 用於存放選擇的新種群
    int currentSize = 0; // 當前新種群的大小

    // 遍歷每個非支配層
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