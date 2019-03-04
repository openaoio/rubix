import ctypes

func = ctypes.CDLL("./librubix.so")

x = func.hello() 
print("in python:")

p = func.rubix_cube_allocate_solved()

#print(p)

func.rubix_cube_print_ascii_stdout(p)
func.print_first(p)
func.rubix_cube_print_ascii_stdout(p)
#func.rubix_cube_print_ascii_stdout(p)
func.rubix_cube_free(p)

# cube = func.rubix_cube_generate_solved()


