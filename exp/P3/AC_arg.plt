set terminal png
set output "CBPF_Agraf.png"
set xrange [500:5000]
set xtics (100,200,500,1000,1250,1500,1750,2000,5000,10000)
set xtics rotate by 315
set logscale x
set yrange [-180:180]
set grid ytics xtics
set ytics 45
set xlabel "Frequency[Hz]"
set ylabel "Phase_difference[degree]"
set nokey
plot "(2)cheBPFAC.txt" using 1:3 with line
