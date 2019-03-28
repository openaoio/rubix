#include "monkey.h"

#include <lil_test.h>
#include <stdlib.h>

typedef enum bool {false,true} bool ;

#define no_op (void)0 ;

#define monkey_lives no_op

#define 	monkey_stillborn(infant_monkey) 				\
										\
		LAMBDA(void,(void) {						\
			monkey_death(&infant_monkey) ;				\
			birth_is_stillborn = true ;				\
		})()

#define 	free_if_valid(ptr)						\
										\
		LAMBDA(void,(void) {						\
			if(*ptr) { 						\
				free(*ptr) ; 					\
				*ptr = NULL ;					\
			}	 						\
		})()

#define 	generic_creator(type) 						\
										\
		LAMBDA(type,(void) {						\
			type new_object ;					\
										\
			new_object = NULL ;					\
			new_object = (type)malloc(sizeof(typeof(*new_object))) ;\
										\
			return new_object ;					\
		})()								\

#define 	monkey_creator()						\
										\
		LAMBDA(Monkey *,(void) {					\
			return generic_creator(Monkey *) ;			\
		})()								

#define 	birth_of(id)							\
										\
		LAMBDA(bool,(void) {						\
			bool birth_is_stillborn ;				\
										\
			birth_is_stillborn = false ;				\
			id = _Generic( (id),					\
				Monkey *:	monkey_creator() 		\
				default:	generic_creator(typeof(id))	\
			) ;							\
										\
			! id							\
			? monkey_stillborn(infant_monkey)			\
			: monkey_lives						\
										\
			return birth_is_stillborn ;				\
		})()

//TODO
Monkey * monkey_birth() {
	Monkey * infant_monkey ;
	bool is_stillborn ;

	is_stillborn = false ;

	is_stillborn = birth_of(infant_monkey) ;
	//is_stillborn = birth_of(infant_monkey->cube) ;

	
	if(is_stillborn)
		infant_monkey = NULL ;

	return infant_monkey ;
}

void monkey_death(Monkey ** target_monkey_location) {
	free_if_valid(target_monkey_location) ;
}
