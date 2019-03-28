#include "monkey.h"

#include <lil_test.h>
#include <stdlib.h>

#define no_op (void)0 ;
#define monkey_stillborn(infant_monkey) ({ monkey_death(&infant_monkey) ; goto afterbirth ;})

#define free_if_valid(ptr) LAMBDA(void,(void) {		\
		if(*ptr) { 				\
			free(*ptr) ; 			\
			*ptr = NULL ;			\
			} 				\
		})()

//TODO
Monkey * monkey_birth() {
	Monkey * infant_monkey ;

	infant_monkey = NULL ;

	infant_monkey = (Monkey *)malloc(sizeof(Monkey)) ;

	! infant_monkey
	? monkey_stillborn(infant_monkey)
	: no_op
	


afterbirth:
	return infant_monkey ;
}

void monkey_death(Monkey ** target_monkey_location) {
	free_if_valid(target_monkey_location) ;
}
