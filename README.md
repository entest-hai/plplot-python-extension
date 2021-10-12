![grid](https://user-images.githubusercontent.com/20411077/136901274-c7dc0bfd-1c8f-432e-b00c-0bdf2c9e48f8.png)
[grid.pdf](https://github.com/tranminhhaifet/python-extension-ctypes/files/7327364/grid.pdf)
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
- build
 ```
 gcc -o app test_plplot.c -I /usr/local/include/ -I /usr/local/include/plplot/ -lm -lgsl -lgslcblas -lplplot
```
- convert ps to png
 ```
 gs -dSAFER -dBATCH -dNOPAUSE -dEPSCrop -r200 -sDEVICE=png16m -sOutputFile=grid.png grid.ps
```
### Build python extension module with Ctypes
- Import ctypes and load the share lib as 
```
lib = ctypes.CDLL('./../build/libtest.so') 
```
- Define data types and pass intputs from python to C extension as 
```
lib.plot_ctg_paper.argtypes = [ctl.ndpointer(np.float32),ctl.ndpointer(np.float32),ctl.ndpointer(np.float32),ctypes.c_int]
```
- Call the extension function 
```
lib.plot_ctg_paper(
    np.array([90.0 for x in range(numHeartRate)],dtype=np.float32),
    np.array([150.0 for x in range(numHeartRate)],dtype=np.float32),
    np.array([80.0 for x in range(numHeartRate)],dtype=np.float32),
    numHeartRate)
```
### Usage example 
- Option 1. Pure C program. cd test and ./run  
- Option 2. Python extension. 
    - cd build, cmake ./../ & make 
    - cd test and python test.py 
  
