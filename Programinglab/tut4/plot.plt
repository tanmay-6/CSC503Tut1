set datafile separator ","
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output "dfm.png"
set style data histograms
set style fill solid 0.6
set xlabel "Threads (M)"
set ylabel "Time (sec)"
set title "Execution time vs Threads"
set xtics rotate by -45
plot "dfm.csv" using 2:xtic(1) title "Time"

set datafile separator ","
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output "dfn.png"
set style data histograms
set style fill solid 0.6
set xlabel "Input size (N)"
set ylabel "Time (sec)"
set title "Execution time vs Number of input"
set key outside top center horizontal
plot "dfn.csv" using 2:xtic(1) title "Time"
