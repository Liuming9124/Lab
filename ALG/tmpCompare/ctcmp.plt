set terminal png font arial 16 size 800,600
set key bmargin

set logscale y
set xlabel 'gen'
set ylabel 'value'
set output 'crcmp.png'

plot    '../2009-jade/crcmp_0.05/1/Paper10_GEN1000_DIM1000_NP100_FESS100000.txt'  pt 1 lw 3 ps 1 lc rgb "blue" title 'Paper' with dots ,\
      '../2009-jade/crcmp_0.05/1/Type0_10_GEN1000_DIM1000_NP100_FESS100000.txt'  pt 1 lw 4 ps 1 lc rgb "red" title 'Type0' with dots ,\
     '../2009-jade/crcmp_0.05/1/Type1_10_GEN1000_DIM1000_NP100_FESS100000.txt'  pt 1 lw 2 ps 1 lc rgb "green" title 'Type1' with dots,\
     '../2009-jade/crcmp_0.05/1/Type2_10_GEN1000_DIM1000_NP100_FESS100000.txt'  pt 1 lw 2 ps 1 lc rgb "black" title 'Type2' with dots,

set output