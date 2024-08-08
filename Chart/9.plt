set terminal png font arial 14 size 800,600
set key bmargin

#set xrange [0:10000]
#set yrange [-1:10]
set logscale y

set xlabel 'iteration'
set ylabel 'value'
set output 'f9.png'

plot  '..\ALG\1997-de\result\result9.txt' pt 1 lw 3 ps 1 lc rgb "blue" title 'de' with lines ,\
    '..\ALG\2009-jade\result\result9.txt' pt 1 lw 3 ps 1 lc rgb "red" title 'jade' with lines ,\
   '..\ALG\2013-shade\result\result9.txt' pt 1 lw 2 ps 1 lc rgb "green" title 'shade' with lines,

set output