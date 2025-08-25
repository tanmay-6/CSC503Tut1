set datafile separator ","
set terminal pngcairo size 1200,700 enhanced font 'Arial,12'
set output "varn.png"
set title "VARN"
set style data histograms
set style histogram cluster gap 1
set style fill solid border -1
set boxwidth 0.9
set key outside top center horizontal
set xtics ("2n" 0, "nlogn" 1, "n√n" 2, "nC2" 3)
set ylabel "Time (s)"
set y2label "Memory (KB)"
set y2tics
set ytics nomirror
plot \
    'varn.csv' using (strcol(2) eq "DFSI" ? $4 : NaN):xtic(3) title "DFSI Time" lc rgb "blue", \
    '' using (strcol(2) eq "DFSI" ? $5 : NaN) axes x1y2 title "DFSI Mem" lc rgb "blue" dt 2, \
    '' using (strcol(2) eq "DFSR" ? $4 : NaN) title "DFSR Time" lc rgb "red", \
    '' using (strcol(2) eq "DFSR" ? $5 : NaN) axes x1y2 title "DFSR Mem" lc rgb "red" dt 2, \
    '' using (strcol(2) eq "BFS"  ? $4 : NaN) title "BFS Time" lc rgb "green", \
    '' using (strcol(2) eq "BFS"  ? $5 : NaN) axes x1y2 title "BFS Mem" lc rgb "green" dt 2
set output "varm.png"
set title "Graph Traversal Performance (VARM)"
set style data linespoints
set key outside top center horizontal
set xtics rotate by -45
set xlabel "Number of Nodes"
set ylabel "Time (s)"
set y2label "Memory (KB)"
set y2tics
set ytics nomirror
plot \
    'varm.csv' using (strcol(2) eq "DFSI" ? $3 : NaN):(strcol(2) eq "DFSI" ? $4 : NaN) title "DFSI Time" lc rgb "blue", \
    '' using (strcol(2) eq "DFSI" ? $3 : NaN):(strcol(2) eq "DFSI" ? $5 : NaN) axes x1y2 title "DFSI Mem" lc rgb "blue" dt 2, \
    '' using (strcol(2) eq "DFSR" ? $3 : NaN):(strcol(2) eq "DFSR" ? $4 : NaN) title "DFSR Time" lc rgb "red", \
    '' using (strcol(2) eq "DFSR" ? $3 : NaN):(strcol(2) eq "DFSR" ? $5 : NaN) axes x1y2 title "DFSR Mem" lc rgb "red" dt 2, \
    '' using (strcol(2) eq "BFS"  ? $3 : NaN):(strcol(2) eq "BFS"  ? $4 : NaN) title "BFS Time" lc rgb "green", \
    '' using (strcol(2) eq "BFS"  ? $3 : NaN):(strcol(2) eq "BFS"  ? $5 : NaN) axes x1y2 title "BFS Mem" lc rgb "green" dt 2