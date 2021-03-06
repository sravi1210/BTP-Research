set terminal pdf enhanced
set output 'Set1_Rho_VS_Cost.eps'

set style data histogram
set style histogram cluster gap 1

set style fill solid border rgb "black"
set key inside top left
set auto x
set yrange [0:*]
set ylabel 'Total EDD cost'
set xlabel 'Destination edge server density ({/Symbol r})'

plot 'N4.dat' using 2:xtic(1) title 'Integer Programming', \
        '' using 3:xtic(1) title 'EDD-NSTE', \
        '' using 4:xtic(1) title 'EDD-A', \
        '' using 5:xtic(1) title 'Greedy', \
        '' using 6:xtic(1) title 'Random'