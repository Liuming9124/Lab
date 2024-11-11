set terminal png font arial 16 size 800,600
set key bmargin

set logscale y
set xlabel 'evaluation'
set ylabel 'value'
set output 'three.png'

plot  '/home/kml/lab/ALG/2009-jade/result/result10_GEN2000_DIM30_NP100_FESS200000.txt'    pt 1 lw 3 ps 1 lc rgb "blue"  title 'jade'   with linespoints,\
      '/home/kml/lab/ALG/2013-shade/result/result10_GEN2000_DIM30_NP100_FESS200000.txt'   pt 1 lw 3 ps 1 lc rgb "green" title 'shade'  with linespoints,\
      '/home/kml/lab/ALG/2014-lshade/result/result10_DIM30_NP100_Fess200000.txt'          pt 1 lw 3 ps 1 lc rgb "red"   title 'lshade' with linespoints,
     

set output