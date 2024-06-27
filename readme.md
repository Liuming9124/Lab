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