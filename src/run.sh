rm app
gcc -o app test_plplot.c -I /usr/local/include/ -I /usr/local/include/plplot/ -lm -lgsl -lgslcblas -lplplot
./app
ps2pdf grid.ps grid.pdf
okular grid.pdf