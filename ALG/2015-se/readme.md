# SE

### Introduce
- Search Economy (SE)，超啟發式演算法可以在足夠的時間內找到好的解，但如果搜尋過程中無法有效利用運算資源，將會浪費許多資源在不夠好的解空間。
- SE的基本想法是根據搜尋演算法檢查過的解來描述解空間，並利用「解空間資訊」來搜尋收斂過程中的解。基於這些概念，搜尋過程的投入將更有意義，因此在早期迭代時不易陷入局部最優。


## SE方法
- Main Method
```
    {
1.      s = init()
2.      m = resource_arrangement
3.      while (!end){
4.          s = vision_search(s, m)
5.          m = marketing_research(s, m)
6.      }
7.      end
8.      Output s
    }

```

- Input Parameter 
- [Run][Func][Evals][Bits][Searchers][Regions][Samples][Players][Iteration]
    - Evals : evaluation times.
    - Bits : Dim
    - Searchers : Searcher number
    - Regions : Region number, must be power of two
    - Samples : Samples in each region
    - Players : Each Searcher will send to new selected region by compare `player times` in expected_value of region.

- Algorithm Parameter
    - ta : number of times the j-th region has been invested
    - tb : number of times the j-th region has not been invested
    - T_J : 代表J區域的搜尋頻率 -> `ta[j] / tb[j]`
    - M_J : 最優解在所有區域的相對適應度，反映該區域的搜尋潛力 -> `sample_sol_best_fitness[j] / total_sample_fitness;`

- Init
    - **初始化資料**
    - (A)
    - 每個Searcher會搜索各自負責的Region
    - 每個Region會有Sample個樣本
    - (B)
    - Searcher_Sol 是演算法中的核心，表示搜尋者當前正在探索的解。
    - Sample_Sol   是區域內的初始解，幫助生成候選解，並促進區域內的探索。
    - SampleV_Sol  是搜索過程中生成的潛在新解，通過信息交換和變異生成，並可能成為新的搜尋者解。
- Resource_Arrangement
    - **切分區域**
    - 在OneMax Problem中，透過切割bit數量來表示Region。*ex: dim=10, region=8, region占用3bit, 搜索樣本占用7bit, 相同region的樣本其region占用的bit相同。*
    - 初始化sample
    - 分配每個Searcher的初始Region
- Vision Search
    - 包含三步驟，**交配->計算期望值->更新Searcher下次疊代的搜尋Region**
    1. 交配 (if eval != 0)
        - 將潛在新解更新後暫存至SampleV
        - 選定一個點進行交配，交配後會出現兩個潛在解
            - 潛在解1 : [region_bit, searcher,   sample]
            - 潛在解2 : [region_bit,   sample, searcher]
        - 對於每一個sample皆隨機挑選一個bit反轉
    2. 計算期望值
        - (if eval==0) 計算每個searcher的fitness並存到saercher_sol_fit 
        - (else) 計算SampleV裡每個sample的fitness
        - (if eval==0) 計算每個sample的fitness
        - 更新區域最佳解
        - 計算 M_J 及 T_J
        - 透過 M_J 及 T_J 計算每個搜索者對於每個區域的 Expected_value
    3. 更新Searcher的Region (Vision_selection)
        - 每個區域的tb值加一
        - 每個搜索者進行num_Player次數的區域選擇，並挑選出期望值較高的區域進行下次搜索，且根據挑選的區域調整ta及tb的值
- Marketing Research
    - 更新 ta 及 tb 值
    - 從Searcher及Sample裡找出最好的解與Best比較並更新
