# 29 SEP 2021 Hai Tran
# 30 SEP 2021 Hai Tran
# add test plplot and wrap python
import ctypes 
import numpy.ctypeslib as ctl 
import numpy as np
from numpy.lib.index_tricks import CClass 

lib = ctypes.CDLL('./libtest.so')
# pass two number
lib.add.argtypes = [ctypes.c_int,ctypes.c_int]
# pass an array 
lib.sum.argtypes = [ctl.ndpointer(np.int), ctypes.c_int]
# pass multiple array 
lib.plot_ctg.argtypes = [ctl.ndpointer(np.float64),ctl.ndpointer(np.float64),ctl.ndpointer(np.float64),ctypes.c_int]
lib.plot_ctg.restype = ctypes.c_int
# test return 
lib.mul.argtypes = [ctypes.c_int,ctypes.c_int]
lib.mul.restype = ctypes.c_int
# call print 
lib.myprint()
# call add 
lib.add(100,100)
# call sum 
lib.sum(np.array([1,2,3,4,5,6,7,8,9,10]),10)
# call mul 
x = lib.mul(100,100)
print("mul = {0:5.0f}".format(x))
# call plot_ctg 
lib.plot_ctg(
    np.array([60.1,60.2,60.3,60.4,60.5,60.6,60.7,60.8,60.9,70.0]),
    np.array([60.1,60.2,60.3,60.4,60.5,60.6,60.7,60.8,60.9,70.0]),
    np.array([60.1,60.2,60.3,60.4,60.5,60.6,60.7,60.8,60.9,70.0]),
    10
)
# test plplot
lib.testPlplot()