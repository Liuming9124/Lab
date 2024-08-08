set terminal png font arial 14 size 800,600
set key bmargin

#set xrange [0:10000]
#set yrange [-1:10]
set logscale y

set xlabel 'iteration'
set ylabel 'value'
set output 'f12.png'

plot '..\2013-shade\result\result12.txt' pt 1 lw 3 ps 1 lc rgb "green" title 'shade' with lines,\
     '..\2014-lshade\result\result12.txt' pt 1 lw 3 ps 1 lc rgb "black" title 'lshade' with lines,

set output