set terminal png
set output "CBPFstep.png"
set xrange [0:0.01]
set yrange [-0.15:0.15]
set grid
set nokey
set xlabel "Time[second]"
set ylabel "V_out[V]"
plot "(2)cheBPFstep.txt" with line
