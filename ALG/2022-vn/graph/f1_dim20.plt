set terminal png font arial 16 size 800,600
set key bmargin

#set xrange [0:10000]
#set yrange [-1:10]
set logscale y

set xlabel 'iteration'
set ylabel 'value'
set output 'f1dim20.png'

plot    '..\result\result1_DIM20_NetLen15_Fess400000.txt' pt 1 lw 3 ps 1 lc rgb "black" title 'VN f1 dim20' with lines,

set output