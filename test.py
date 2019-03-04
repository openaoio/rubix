import ctypes

func = ctypes.CDLL("./librubix.so")

x = func.hello() 
print("in python:")

func.makeprint()

# cube = func.rubix_cube_generate_solved()


