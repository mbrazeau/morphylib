from ctypes import *
cdll.LoadLibrary("../libmorphy_x86_64-Darwin.so")
morphylib = CDLL("../libmorphy_x86_64-Darwin.so");

morphylib.mpl_new_Morphy.argtypes = []
morphylib.mpl_new_Morphy.restype = c_void_p

morphylib.mpl_delete_Morphy.argtypes = [c_void_p]
morphylib.mpl_delete_Morphy.restype = c_int

morphylib.mpl_init_Morphy.argtypes = [c_int, c_int, c_void_p]
morphylib.mpl_init_Morphy.restype = c_int




