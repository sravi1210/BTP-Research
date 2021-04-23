# Scale font and line width (dpi) by changing the size! It will always display stretched.
set terminal pdf font 'Verdana,15'
set output 'N.pdf'
# Key means label...
set key inside top left
set xlabel 'Number of nodes N in the graph G'
set ylabel 'Total EDD cost'
plot  "N.dat" using 1:2 title 'Integer Programming' with lines, "N.dat" using 1:3 title 'EDD-NSTE' with linespoints,"N.dat" using 1:4 title 'EDD-A' with linespoints,"N.dat" using 1:5 title 'Greedy' with linespoints,"N.dat" using 1:6 title 'Random' with linespoints