rm app
gcc -o app ./../src/plplot_ctg_paper.c -I /usr/local/include/ -I /usr/local/include/plplot/ -lm -lgsl -lgslcblas -lplplot
./app
gs -dSAFER -dBATCH -dNOPAUSE -dEPSCrop -r200 -sDEVICE=png16m -sOutputFile=grid.png grid.ps
gs -dSAFER -dBATCH -dNOPAUSE -dEPSCrop -sDEVICE=pdfwrite -g5775x6207 -dPDFFitPage -o grid.pdf grid.ps
gs -dSAFER -dBATCH -dNOPAUSE -dEPSCrop -sDEVICE=pdfwrite -dPDFFitPage -o grid.pdf grid.ps
okular grid.pdf