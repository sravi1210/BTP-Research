# Scale font and line width (dpi) by changing the size! It will always display stretched.
set terminal pdf font 'cmr10.tfm,10'
set term post enhanced color
set output 'Set1_LC_VS_Cost.eps'
# Key means label...
set key inside bottom left
set xlabel 'EDD length constraint L_{limit}'
set ylabel 'Total EDD cost'
plot  "N3.dat" using 1:2 title 'Integer Programming' with lines linecolor rgb '#8CB500' linewidth 1.5,"N3.dat" using 1:3 title 'EDD-NSTE' with linespoints linecolor rgb '#9966CC' linewidth 1.5,"N3.dat" using 1:4 title 'EDD-A' with linespoints linecolor rgb '#CC1942' linewidth 1.5,"N3.dat" using 1:5 title 'Greedy' with linespoints linecolor rgb '#002E63' linewidth 1.5,"N3.dat" using 1:6 title 'Random' with linespoints linecolor rgb '#184343' linewidth 1.5
