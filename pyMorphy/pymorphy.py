from ctypes import *
cdll.LoadLibrary("../libmorphy_x86_64-Darwin.so")
Morphy.CDLL("../libmorphy_x86_64-Darwin.so");

Morphy.mpl_new_Morphy.argtypes = []
Morphy.mpl_new_Morphy.restype = c_void_p

Morphy.mpl_delete_Morphy.argtypes = [c_void_p]
Morphy.mpl_delete_Morphy.restype = c_int



