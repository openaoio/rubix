import ctypes
import sys
import re

librubix = ctypes.CDLL("./librubix.so")

# seed = librubix.rubix_cube_generate_seed()

# cuberef = librubix.rubix_cube_allocate_scrambled(seed)
# librubix.rubix_cube_print_ascii_double_stdout(cuberef)

# librubix.rubix_cube_solve_scrambled_from_seed(cuberef,seed)
# librubix.rubix_cube_print_ascii_double_stdout(cuberef)

librubix.rubix_cube_print_ascii_double_stdout.argtypes = [ctypes.c_void_p]

librubix.rubix_cube_allocate_scrambled.restype = ctypes.c_void_p
librubix.rubix_cube_allocate_solved.restype = ctypes.c_void_p

librubix.rubix_cube_free.argtypes = [ctypes.c_void_p]
librubix.rubix_cube_scramble_free.argtypes = [ctypes.c_void_p]

librubix.rubix_cube_scramble_allocate.restype = ctypes.c_void_p
librubix.rubix_cube_scramble_free.argtypes = [ctypes.c_void_p]

librubix.rubix_cube_apply_scramble.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
librubix.rubix_cube_unapply_scramble.argtypes = [ctypes.c_void_p, ctypes.c_void_p]

# librubix.rubix_cube_free(cuberef)

class RubixCube:

    FACES = [
            "top",
            "front",
            "right",
            "left",
            "back",
            "bottom"
            ]

    ROTATIONS = [
            "clockwise",
            "counterclockwise",
            "double"
            ]

    DEFAULT_SCRAMBLE_INTENSITY = librubix.rubix_cube_get_default_scramble_intensity()

    def __init__(self,seed):
        if seed == 0 or seed == None:
            self.ptr = librubix.rubix_cube_allocate_solved()
        else:
            self.ptr = librubix.rubix_cube_allocate_scrambled(int(seed))
        self.scramble_ptrs = []

    def __del__(self):
        librubix.rubix_cube_free(self.ptr)
        for ptr in self.scramble_ptrs:
            librubix.rubix_cube_scramble_free(ptr)

    def __eq__(self, other):
        return librubix.rubix_cube_equivelence_check(self.ptr,other.ptr) == True

    def is_solved(self):
        return librubix.rubix_cube_is_solved(self.ptr) == True

    def print3D(self):
        librubix.rubix_cube_print_ascii_double_stdout(self.ptr)

    def rotate(self,face,rotation):
        if not face in self.FACES or not rotation in self.ROTATIONS:
            return None
        librubix.rubix_cube_rotate_face(self.ptr, self.FACES.index(face), self.ROTATIONS.index(rotation))

    def scramble(self,intensity = DEFAULT_SCRAMBLE_INTENSITY, seed = 0):
        self.scramble_ptrs.append(librubix.rubix_cube_scramble_allocate(seed,intensity))
        librubix.rubix_cube_apply_scramble(self.ptr, self.scramble_ptrs[-1])

    def unscramble(self, history_depth = 1):
        if len(self.scramble_ptrs) == 0:
            print("No scrambles in scramble history. Doing nothing.")
            return
        librubix.rubix_cube_unapply_scramble(self.ptr, self.scramble_ptrs[-1 * history_depth])
        librubix.rubix_cube_scramble_free(self.scramble_ptrs.pop())

    def reset(self):
        librubix.rubix_cube_free(self.ptr)
        self.ptr = librubix.rubix_cube_allocate_solved()

class CubeShell:
    def exit(self,tokens):
        self.do_exit = True

    def show_help(self,tokens):
        print("\
exit:       quit the program\n\
help:       show this message\n\
new:        start a new cube, discarding the old one\n\
rotate:     rotate a side by a rotation value\n\
      sides = top, front, right, left, back, bottom\n\
  rotations = clockwise (c), counterclockwise (cc), double(d)\n\
print:      display a 3D representation of the cube\n\
scramble:   scramble the cube and save to scramble history\n\
unscramble: unscramble the cube based on most recent scramble\n\
check:      check if the cube is solved\n\
history:    execute previous commands, defaults to previous\n\
          n = steps back in history (max 50)\n\
!           alias for history\n"
            )
        return

    def new(self,tokens):
        self.cube.reset()
        self.cube.print3D()
        return

    ROTATION_ALIASES =  {
            "c"     :"clockwise",
            "cw"    :"clockwise",
            "cl"    :"clockwise",
            "cc"    :"counterclockwise",
            "ccl"   :"counterclockwise",
            "ccw"   :"counterclockwise",
            "d"     :"double",
            "dd"    :"double",
            "ddd"   :"double"
            }

    def disambiguate_query(self,query,options):
        new_options = options
        for i in range(len(query)):
            for j in range(len(options)):
                if query[i] != options[j][i]:
                    new_options.remove(options[i][i])
            if len(new_options) == 1:
                return new_options[0]
            elif len(new_options) == 0:
                return None

    def rotate(self,tokens):
        face = "invalid"
        rotation = "invalid"
        
        if len(tokens) < 3:
            print("Rotation failed: not enough arguments")
        
        if tokens[1] not in self.cube.FACES:
            print("Rotation failed: invalid face")
            return
        else:
            face = tokens[1]

        if tokens[2] not in self.cube.ROTATIONS:
                try:
                    rotation = self.ROTATION_ALIASES[tokens[2]]
                except:
                    print("Rotation failed: invalid rotation")
                    return
        else:
            rotation = tokens[2]
        self.cube.rotate(face,rotation)
        self.cube.print3D()

    def print3D(self,tokens):
        self.cube.print3D()
        return

    def scramble(self,tokens):
        self.cube.scramble()
        self.cube.print3D()
        return

    def unscramble(self,tokens):
        self.cube.unscramble()
        self.cube.print3D()
        return

    def check(self,tokens):
        if self.cube.is_solved():
            print("The cube is solved")
        else:
            print("The cube is scrambled")

    MAXIMUM_ARCHEOLOGY = 50 # arbitrary
    def historic_execution(self,tokens):
        #print("depth: %d" % self.historic_execution_depth)

        if self.historic_execution_depth >= self.MAXIMUM_ARCHEOLOGY:
            print("Cannot execute hisorical command: out of our depths")
            return
        self.historic_execution_depth += 1

        scalar = 1
        if len(tokens) > 1:
            try:
                scalar = int(tokens[1])
            except:
                None
        #print(len(self.history))
        if scalar >= len(self.history):
            print("Cannot go %d steps back: not enough history" % scalar)
            return

        offset = (scalar * -1) - self.historic_execution_depth
        
        #print(offset)
        print("\"%s\"" % self.history[offset])
        self.parse(self.history[offset])

        self.historic_execution_depth -= 1

    def default(self):
            if len(self.history) > 1 + self.historic_execution_depth:
                self.historic_execution([])

    COMMANDS = {
            "exit"      :exit,
            "help"      :show_help,
            "new"       :new,
            "rotate"    :rotate,
            "print"     :print3D,
            "scramble"  :scramble,
            "unscramble":unscramble,
            "check"     :check,
            "history"   :historic_execution,
            "!"         :historic_execution
            }

    PS1 = ">> "

    def parse(self,raw_command):
        if not raw_command:
            self.default()
        else:
            try:
                tokens = raw_command.lower().split()
                fun = self.COMMANDS[tokens[0]]
            except:
                self.show_help(None)
            else:
                fun(self,tokens)


    def __init__(self):
        self.do_exit = False 
        self.historic_execution_depth = 0
        self.cube = RubixCube(0)
        self.history = []
        self.cube.print3D()
        self.input_loop()

    def input_loop(self):
        while self.do_exit == False:
            sys.stdout.write(self.PS1)
            self.history.append(input())
            self.parse(self.history[-1])
        return

    @classmethod
    def hello(shell):
        print("[[CUBE SHELL VERSION 0.1]]")
        print("Generating a new, solved cube...")

    @classmethod
    def launch(shell):
        shell.hello()
        return CubeShell()


if __name__ == "__main__":
    CubeShell.launch()

    # TODO
    #def disambiguate_querry(query,options):
    #    new_options = options
    #    for i in range(len(query)):
    #        for j in range(len(options)):
    #            if i >= len(options[j]):
    #               continue 
    #            elif query[i] != options[j][i]:
    #                new_options.remove(options[j])
    #            #print(options[j][i])
    #        print(new_options)
    #        if len(new_options) == 1:
    #            return new_options[0]
    #        elif len(new_options) == 0:
    #            return None

#print(disambiguate_querry("to",RubixCube.FACES))
