set terminal png font arial 14 size 800,600
set key bmargin

#set xrange [0:10000]
set yrange [-1:10]

set xlabel 'iteration'
set ylabel 'value'
set output 'jade.png'

plot './result/result1.txt' pt 1 lw 1 ps 1 lc rgb "blue" title 'jade'

set output