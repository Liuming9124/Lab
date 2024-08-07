set terminal png font arial 14 size 800,600
set key bmargin

#set xrange [0:10000]
#set yrange [-1:10]
set logscale y

set xlabel 'iteration'
set ylabel 'value'
set output 'f4.png'

plot  '..\ALG\1997-de\result\result4.txt' pt 1 lw 3 ps 1 lc rgb "blue" title 'de' with dots ,\
    '..\ALG\2009-jade\result\result4.txt' pt 1 lw 3 ps 1 lc rgb "red" title 'jade' with dots ,\
   '..\ALG\2013-shade\result\result4.txt' pt 1 lw 3 ps 1 lc rgb "green" title 'shade' with dots,

set output