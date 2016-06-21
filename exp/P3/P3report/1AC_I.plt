set terminal png
set output "1AC_Igraf.png"
set xrange [0:5000000]
set xlabel "Frequency[Hz]"
set ylabel "I_R2[A]"
plot "(1)(a).txt" using 1:2 with line
