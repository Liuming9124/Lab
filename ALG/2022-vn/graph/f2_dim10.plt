set terminal png font arial 16 size 800,600
set key bmargin

#set xrange [0:10000]
#set yrange [-1:10]
set logscale y

set xlabel 'iteration'
set ylabel 'value'
set output 'f2dim10.png'

plot    '..\result\result2_DIM10_NetLen15_Fess200000.txt' pt 1 lw 3 ps 1 lc rgb "green" title 'VN f2 dim10' with lines,\

set output