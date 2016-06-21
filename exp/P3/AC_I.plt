set terminal png
set output "BLPF_Vgraf.png"
set xrange [100:5000]
set yrange [-40:5]
set xtics (100,200,500,1000,1500,2000,5000)
set ytics 3
set grid ytics mxtics xtics
set logscale x
set xlabel "Frequency[Hz]"
set ylabel "V_out/V_in[dB]"
set nokey
plot "(2)ButLPFAC.txt" using 1:2 with line
