set terminal png
set output "1AC_Agraf.png"
set xrange [0:5000000]
set yrange [-180:180]
set xlabel "Frequency[Hz]"
set ylabel "I_R2 Phase_difference[degree]"
plot "(1)(a).txt" using 1:3 with line
