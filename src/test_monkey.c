#include <lil_test.h>
#include "monkey.h"
#include "rubix_cube.h"

TEST_SET(create_destroy,
	TEST_CASE(default,
		Monkey * joe = monkey_birth() ;
		printf("monkey is at %p\n",joe) ;
		printf("cube is at %p\n",joe->cube) ;
		rubix_cube_print_ascii_double_stdout(joe->cube) ;
		monkey_death(&joe) ;
	)

)

TEST_MAIN()
