# python-extension-ctypes
wrap some c functions which using plplot to draw things, then can be called as python code. 
### Install plplot
- download plplot from here: http://plplot.sourceforge.net/downloads.php 
- mkdir build_dir then cd build_dir, cmake .., then sudo make and sudo make install 
- check ls /usr/local/lib/ and /usr/local/include/ 
- TODO: setup and install device such as pngcairo? 
### Draw a simple grid with plplot 
- set output plsfname("./test.ps")
- set device plsdev("ps")
- option: 
    - set page plspage()
    - subpage plsadv()
    - viewport plsvap(xmin,xmax,ymin,ymax) in mm 
    - viewport aspect ratio plsvaps(xmin,xmax,ymin,ymax,0.0) 0.0 to 1.0 
    - window plswind(xmin,xmax,ymin,ymax) data axes 
- simple set env to plot plsenv(xmin,xmax,ymin,ymax)
- plot label pllab()
- plot line plline() 
- build: gcc -o app test_plplot.c -I /usr/local/include/ -I /usr/local/include/plplot/ -lm -lgsl -lgslcblas -lplplot
- convert ps to png: gs -dSAFER -dBATCH -dNOPAUSE -dEPSCrop -r200 -sDEVICE=png16m -sOutputFile=grid.png grid.ps
### Ctype 
### Test 
