# 2013-Shade

## Key Point

- Base on Jade
- Remove P & C parameter to adapt with itself
- Add History Memorey OF MCR & MF
- Randomly select MCR&MF from History Memory
- SHADE does not have an explicit learning rate parameter – instead, the memory size H plays a similar role.
- the rate of control parameter convergence is expected to slow down because older parameters will continue to have influence for a longer time.
- 增加容錯率(Compare to Jade)
## How to Run
```g++ main_shade.cpp```

```./a.exe [Run] [Func] [NP] [FESS] [Dim] [Arch] [H]```
- NP : 100(paper recommended)
- FESS : NP*GEN
- Arch : 1->size with NP , 0-> size with 0
- H : size of success history

EX:

```R 1 30 150000 30 1 100```
```R 2 30 60000  30 1 100```
```R 3 30 150000 30 1 100```
```R 4 30 150000 30 1 100```


- fun5 (gen 3000 & 20000):

```R 5 30 90000  30 1 100```
```R 5 30 600000 30 1 100```

- fun6 (gen 100 & 1500)
```R 6 30 3000  30 1 100```
```R 6 30 45000 30 1 100```


```R 7 30 90000 30 1 100```

- fun8 (gen 1000 & 9000)
```R 8 30 30000  1 100```
```R 8 30 270000 1 100```

- fun9 (gen 1000 & 5000)
```R 9 30 30000  1 100```
```R 9 30 150000 1 100```

- fun10 (gen 500 & 2000)
```R 10 30 15000 1 100```
```R 10 30 60000 1 100```

- fun11 (gen 500 & 3000)
```R 11 30 15000 1 100```
```R 11 30 90000 1 100```

- fun12 (gen 500 & 1500)
```R 12 30 15000 1 100```
```R 12 30 45000 1 100```

- fun13 (gen 500 & 1500)
```R 13 30 15000 1 100```
```R 13 30 45000 1 100```