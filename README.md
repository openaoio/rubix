# Rubix

### A rubix cube simulation library

#### Done (i.e. In testing)

* Rubix cube data representation
* Visual representations of the cube
* Rubix cube transformation algorithms (face rotations)
* Batch move execution
* Random scrambling by seed value
* Unscrambling by seed value
* Builds as shared library
* Interactive rubix cube shell via python

#### In Development:

* A million monkeys
* Enhancements to interactive rubix cube shell via python (e.g. disambiguation)

#### Down the road:

* Save/Load state to/from hard drive
* Rubix cube cube solution algorithm implementation

### How to build the tests (which include some demos):

Initialize and update submodules: `git submodule init && git submodule update`
Build with: `make test`
Run with: `./test`

Clean the directory with `make clean`

### How to actually use the library:

However you damn please, this code is open source.

That in mind, I recommend using the Python API instead of the C API, as it was hacked together in a day or so

But, try out `make && python rubix.py` and see what happens
