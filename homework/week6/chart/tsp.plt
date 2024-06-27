set terminal png size 800,600
set output 'tsp_route.png'

set title "TSP Route"
set xlabel "X"
set ylabel "Y"

set xrange [0:70]
set yrange [0:80]

plot '../map.txt' using 2:3 title 'Nodes' with points pt 7 ps 1, \
     'route.txt' using 2:3 with lines lt 1 lw 2 title 'Path'
