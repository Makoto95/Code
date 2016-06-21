set terminal png
set output "BLPF_Vgraf.png"
set xrange [500:5000]
set xtics (100,200,500,1000,1500,2000,2985,4215,5000,10000)
set xtics rotate by 315
set yrange [-36:3]
set ytics 3
set grid ytics xtics
set logscale x
set xlabel "Frequency[Hz]"
set ylabel "V_out/V_in[dB]"
set nokey
plot "(2)ButLPFAC.txt" using 1:2 with line
