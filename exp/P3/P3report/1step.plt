set terminal png
set output "1stepgraf.png"
set xrange [0:5e-06]
set yrange [-0.0003:0.0003]
set xlabel "Time[second]"
set ylabel "I_R2[A]"
plot "(1)(a)step.txt" with line
