# 29 SEP 2021 Hai Tran
# 30 SEP 2021 Hai Tran
# add test plplot and wrap python
import ctypes
import numpy.ctypeslib as ctl
import numpy as np
from numpy.lib.index_tricks import CClass

# load share lib .so
lib = ctypes.CDLL('./../build/libtest.so')
# prepare inputs for plot
numHeartRate = 45*60*4
# pass input from python to c shared lib
lib.plot_ctg_paper.argtypes = [ctl.ndpointer(np.float32),ctl.ndpointer(np.float32),ctl.ndpointer(np.float32),ctypes.c_int]
# call the extension function
lib.plot_ctg_paper(
    np.array([90.0 for x in range(numHeartRate)],dtype=np.float32),
    np.array([150.0 for x in range(numHeartRate)],dtype=np.float32),
    np.array([80.0 for x in range(numHeartRate)],dtype=np.float32),
    numHeartRate)