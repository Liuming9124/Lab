# Lab Homework
## Folder Structure
- chart
  - plt file
  - png file
- problem (cpp)
  - Ackley
  - Deception
  - OneMax
  - Tsp
- result
  - Problem
    - Alg
      - Result
- week *
  - main.cpp
  - Alg.h

## Usage for each homework
##### (Need change directory to current week folder)
### Week1
1. Exhaustive for OneMax
`g++ -o ex main_exhaustive.cpp`
`./ex.exe [bit] [run] [iteration]`
`./ex.exe 10 5 1000`
    Can see the result file at `homework/result/onemax/exhaustive`
2. Hill for OneMax
`g++ -o hill main_hill.cpp`
`./hill.exe [bit] [run] [iteration]`
`./hill.exe 10 5 1000`

### Week2
1. SA for OneMax
  `g++ -o saom main_saom.cpp`
  `./saom.exe [Bit] [Run] [Iter] [Temp] [Rate]`
  `./saom.exe 10 5 1000 0.9 0.9`

2. SA for Deception
  `g++ -o sadeception main_sadeception.cpp`
  `./sadeception.exe [Bit] [Run] [Iter] [Temp] [Rate]`
  `./sadeception.exe 10 5 1000 0.9 0.9`

### Week3
1. GA for OneMax
  `g++ -o gaom main_gaom.cpp`
  `./gaom.exe [Bit] [Run] [Iter] [Pop] [CR] [MR]`
  `./gaom.exe 50 30 20000 100 0.3 0.3`
2. Tabu Search for OneMax
  `g++ -o tsom main_tsom.cpp`
  `./tsom.exe [Bit] [Run] [Iter] [Tweaks] [Tblen]`
  `./tsom.exe 10 30 1000 100 5`
3. Tabu Search for Deception
  `g++ -o tsde main_tsde.cpp`
  `./tsde.exe [Bit] [Run] [Iter] [Tweaks] [Tblen]`
  `./tsde.exe 10 30 1000 100 5`

### Week4
1. DE for Ackley
  `g++ -o deack main_de.cpp`
  `./deack.exe [Pop] [Run] [Iter] [Dim] [Bounder] [Cr] [Mutation Factor]`
  `./deack.exe 20 5 100000 10 32 0.6 0.8`
2. PSO for Ackley
  `g++ -o psoack main_pso.cpp`
  `./psoack.exe [Pop] [Run] [Iter] [Dim] [Bounder]`
  `./psoack.exe 20 5 100000 10 32`

### Week6
1. Ant for TSP
  `g++ -o ant main_ant.cpp`
  `./ant.exe [fileName] [Run] [iteration] [numAnts] [alpha] [beta] [rho] [Q] [pmute]`
  `./ant.exe map.txt 5 100 50 2 2 1 0.2 0.05 > ./result.txt`
1.1 Draw Chart
    - Copy the Route from ./result.txt to ./chart/route.txt which format would be (id, x, y) and use gnuplot to show it.