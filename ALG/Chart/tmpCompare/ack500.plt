set terminal png font arial 16 size 800,600
set key bmargin

set logscale y
set xlabel 'iteration'
set ylabel 'value'
set output 'ack500.png'

plot    '..\1997-de\result\result0_50000.txt' using ($0*10):1 every 10 pt 1 lw 3 ps 1 lc rgb "blue" title 'de' with linespoints,\
      '..\2009-jade\result\result0_50000.txt' using ($0*10):1 every 10 pt 1 lw 3 ps 1 lc rgb "red" title 'jade' with linespoints ,\
     '..\2013-shade\result\result0_50000.txt' using ($0*10):1 every 10 pt 1 lw 3 ps 1 lc rgb "green" title 'shade' with linespoints,
     

set output