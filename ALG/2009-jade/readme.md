# 2009-Jade
## How to Run
```g++ main_jade.cpp```

```./a.exe [Run] [NP] [Generation] [Dim] [P] [C] [Archieve] [Func]```
- NP: To be 30, 100, and 400 in the case of D ≤ 10, = 30, and = 100, respectively
- Bounder: key in positive number, it'll between [-Bounder, Bounder]
- C: rate of parameter adaptation (0.1)
- P: greediness of the mutation strategy (0.05)
- Archieve : 1->_A.size=NP; 0->_A.size=0
- Func: Choose which function to evaluate, must be positive number

EX:

```.\a.exe 1 20 100000 10 0.05 0.1 1 0```

```.\a.exe 10 30 1500 30 0.05 0.1 1 1```

```.\a.exe 10 30 2000 30 0.05 0.1 1 2```

```.\a.exe 10 30 5000 30 0.05 0.1 1 3```

```.\a.exe 10 30 5000 30 0.05 0.1 1 4```

```.\a.exe 10 30 3000 30 0.05 0.1 1 5```

```.\a.exe 10 30 100 30 0.05 0.1 1 6```

```.\a.exe 10 30 3000 1 30 0.05 0.1 1 7```