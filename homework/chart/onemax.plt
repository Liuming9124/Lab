set terminal png font arial 14 size 800,600
set key bmargin

#set xrange [0:10000]
#set yrange [0:20]

set xlabel 'iteration'
set ylabel 'value'
set output 'onemax.png'

#plot 'temperture_in_iteration.txt' pt 7 lw 2 lc rgb "blue" title 'T-100--CD-0.99'

#plot '../result/onemax/exhaustive/onemaxexhaustive_0.txt' pt 7 lw 2 ps 1 lc rgb "blue" title 'onemaxexhaustive_1',\
#     '../result/onemax/hill/onemaxhill_0.txt' pt 6 lw 2 ps 1.5 lc rgb "red" title 'onemaxhill_0'

# onemax sa
plot '../result/onemax/sa/onemaxsa_0.txt' with lines lw 2 lc rgb "blue" title 'onemaxsa_0'

set output