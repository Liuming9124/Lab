# 2009-Jade
## How to Run
```g++ main_jade.cpp```

```./a.exe [Run] [NP] [Generation] [Bounder] [Dim] [P] [C] [Func]```
- Bounder: key in positive number, it'll between [-Bounder, Bounder]
- C: rate of parameter adaptation (0.1)
- P: greediness of the mutation strategy (0.05)
- Func: Choose which function to evaluate, must be positive number
- NP: To be 30, 100, and 400 in the case of D ≤ 10, = 30, and = 100, respectively

EX:

```.\a.exe 10 30 1500 100 30 0.05 0.1 1```

```.\a.exe 10 30 2000 10 30 0.05 0.1 2```

```.\a.exe 10 30 5000 100  30 0.05 0.1 3```

```.\a.exe 10 30 5000 100 30 0.05 0.1 4```

```.\a.exe 10 30 3000 30 30 0.05 0.1 5```

```.\a.exe 10 30 100 100 30 0.05 0.1 6```

```.\a.exe 10 30 3000 1.28 30 0.05 0.1 7```