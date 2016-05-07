#!/usr/bin/gnuplot
set term pngcairo size 480,200 font "PT Sans,10"
set output "graph.png"
set datafile separator comma
set xdata time
set timefmt '%Y-%m-%dT%H:%M:%SZ'
set yrange [15:35]
set ylabel "Temp[C]"
set y2range [0:100]
set y2label "RH[%]"
set style line 1 lc rgb '#bd0026' lt 1 lw 0.5 # dark red
set style line 2 lc rgb '#41b6c4' lt 1 lw 0.5 # blue-green
plot 'data.csv' u 1:2 w l ls 1 notitle axis x1y1 , \
     'data.csv' u 1:3 w l ls 2 notitle axis x1y2