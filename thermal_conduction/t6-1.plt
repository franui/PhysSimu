set pm3d map #カラーマップのみを表示
set cbrange[1.5:1.7] #カラーバーの範囲の指定
set size square #アスペクト比を正方形にする
set xlabel "x"
set ylabel "y"
set cblabel "T"
#
set multiplot layout 1,2
set lmargin screen 0.1
set rmargin screen 0.4
set tmargin screen 1
set bmargin screen 0
splot "t=1.txt"
set lmargin screen 0.6
set rmargin screen 0.9
set tmargin screen 1
set bmargin screen 0
splot "t=2.txt"
unset multiplot
#
set terminal postscript eps enhanced color
set output "t6-1.eps"
#
set multiplot layout 1,2
set lmargin screen 0.125
set rmargin screen 0.375
set tmargin screen 1
set bmargin screen 0
splot "t=1.txt"
set lmargin screen 0.575
set rmargin screen 0.825
set tmargin screen 1
set bmargin screen 0
splot "t=2.txt"
unset multiplot
#
set terminal "wxt"