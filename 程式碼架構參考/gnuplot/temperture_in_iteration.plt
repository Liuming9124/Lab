set terminal png font arial 14 size 800,600
set key bmargin

#set xrange [0:100]
#set yrange [0:100]

set xlabel 'iteration'
set ylabel 'temperture'
set output 'temperture.png'

#plot 'temperture_in_iteration.txt' pt 7 lw 2 lc rgb "blue" title 'T-100--CD-0.99'

plot 'T_100CD_99.txt' pt 7 lw 2 ps 1 lc rgb "blue" title 'T-100--CD-0.99',\
     'T_100CD_90.txt' pt 6 lw 2 ps 1.5 lc rgb "red" title 'T-100--CD-0.9'

set output