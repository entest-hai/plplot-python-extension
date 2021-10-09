# python-extension-ctypes
wrap some c functions which using plplot to draw things, then can be called as python code. 
### Plplot
- download plplot from here: http://plplot.sourceforge.net/downloads.php 
- mkdir build_dir then cd build_dir, cmake .., then sudo make and sudo make install 
- check ls /usr/local/lib/ and /usr/local/include/ 
- TODO: setup and install device such as pngcairo? 
### Draw grid with Plplot 
- set output plsfname("./test.ps")
- set device plsdev("ps")
- option: set page plspage(), subpage plsadv(), viewport plsvap() viewport aspect ratio plsvaps(), and window plswind()
- simple set env to plot plsenv(xmin,xmax,ymin,ymax)
- pllab() and plline() 
- build: gcc -o app test_plplot.c -I /usr/local/include/ -I /usr/local/include/plplot/ -lm -lgsl -lgslcblas -lplplot
- convert ps to png: gs -dSAFER -dBATCH -dNOPAUSE -dEPSCrop -r200 -sDEVICE=png16m -sOutputFile=grid.png grid.ps
### Ctype 
### Test 
