# 設定輸出圖形格式和文件
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'tsp_solution.png'

# 設定圖形標題和軸標籤
set title "Traveling Salesman Problem Solution"
set xlabel "X-axis"
set ylabel "Y-axis"

# 設定比例為相等，確保圖形不變形
set size ratio -1

# 禁用鍵
unset key

# 繪製城市座標
plot 'tsp_data.txt' index 0 using 2:3 with points pt 7 ps 1.5 lc rgb 'blue', \
     '' index 1 using 2:3 with linespoints lt 1 lw 2 lc rgb 'red'
