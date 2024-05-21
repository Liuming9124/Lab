set terminal png font arial 14 size 800,600
set key bmargin

#set xrange [0:10000]
#set yrange [0:100000000]

set xlabel 'iteration'
set ylabel 'value'
set output 'deception.png'


plot '../result/deception/sa/deceptionsa_0.txt' with lines lw 2 lc rgb "blue" title 'deceptionsa_0'


set output