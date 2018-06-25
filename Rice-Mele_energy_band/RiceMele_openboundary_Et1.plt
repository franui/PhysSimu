set terminal wxt enhanced
unset key
set xlabel 't'
set ylabel 'E(t)'
set xrange[0:2*pi]
set title "band number=20, {/Symbol D}t=2{/Symbol p}/400, J_0=1, {/Symbol d}=1, {/Symbol D}=2"
#
plot for [i=2:21] 'RiceMele_openboundary_Et.txt' using 1:i with l notitle
#
set terminal postscript eps enhanced color
set output "RiceMele_openboundary_Et.eps"
replot
set terminal png
set output "RiceMele_openboundary_Et.png"
replot
set terminal wxt enhanced