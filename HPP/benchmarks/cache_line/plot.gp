set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 6 pi -1 ps 1.5
set pointintervalbox 3
set title "Determination of Cache Line Size From Latency"
set xlabel "Kth Integer"
set ylabel "Time (ms)"
plot "cl_results.txt" with linespoints ls 1 
