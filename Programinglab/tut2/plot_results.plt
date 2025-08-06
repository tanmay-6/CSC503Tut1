set datafile separator ","
set terminal pngcairo size 1000,600 enhanced font 'Verdana,10'

# Time Plot
set output 'time_plot.png'
set title "Average Sorting Time"
set xlabel "Algorithm"
set ylabel "Time (seconds)"
set grid ytics
set style data histograms
set style fill solid 0.5 border -1
set boxwidth 0.5
set xtics rotate by 45
plot 'results.csv' using 2:xtic(1) title "Time (s)" lc rgb "#1f77b4"

# Memory Plot
set output 'memory_plot.png'
set title "Average Memory Usage"
set xlabel "Algorithm"
set ylabel "Memory (KB)"
set grid ytics
set style data histograms
set style fill solid 0.5 border -1
set boxwidth 0.5
set xtics rotate by 45
plot 'results.csv' using 3:xtic(1) title "Memory (KB)" lc rgb "#ff7f0e"
