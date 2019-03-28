#include <lil_test.h>
#include "monkey.h"

TEST_SET(create_destroy,
	TEST_CASE(default,
		Monkey * joe = monkey_birth() ;
		printf("monkey is at %p\n",joe) ;
		monkey_death(&joe) ;
	)

)

TEST_MAIN()
