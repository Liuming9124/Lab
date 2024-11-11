set terminal png font arial 16 size 800,600
set key bmargin

#set xrange [0:10000]
#set yrange [-1:10]
set logscale y


set xlabel 'iteration'
set ylabel 'value'
set output 'f1dim10.png'

plot    '..\result\result1_DIM10_NetLen15_Fess200000.txt' pt 1 lw 3 ps 1 lc rgb "green" title 'VN f1 dim10' with lines,\

set output