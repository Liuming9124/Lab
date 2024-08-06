# 2009-Jade
## How to Run
```g++ main_jade.cpp```
```./a.exe [Run] [Func] [NP] [Fess] [Dim] [Arch] [P] [C]```
- Func: Choose which function to evaluate, must be positive number
- NP: To be 30, 100, and 400 in the case of D ≤ 10, = 30, and = 100, respectively
- Archieve : 1->_A.size=NP; 0->_A.size=0
- P: greediness of the mutation strategy (0.05)
- C: rate of parameter adaptation (0.1)

EX:
```R 1 30 45000 30 0.05 0.1 1```
```R 2 30 60000 30 0.05 0.1 1```
```R 3 30 150000 30 0.05 0.1 1```
```R 4 30 150000 30 0.05 0.1 1```

- fun5 (gen 3000):
```R 5 30 90000 30 0.05 0.1 1```
- fun5 (gen 20000):
```R 5 30 600000 30 0.05 0.1 1```