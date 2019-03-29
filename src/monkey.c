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
			living = false ;					\
		})()

#define 	generic_creator(type) 						\
										\
		LAMBDA(type,(void) {						\
			type new_object ;					\
										\
			new_object = NULL ;					\
			new_object = (type)malloc(sizeof(typeof(*new_object))) ;\
			memset(new_object, 0, sizeof(*new_object)) ;		\
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
		LAMBDA(void,(void) {						\
			id = _Generic( (id),					\
				Monkey *:	monkey_creator(), 		\
				RubixCube *:	rubix_cube_allocate_solved(), 	\
				default:	generic_creator(typeof(id))	\
			) ;							\
										\
			! id							\
			? monkey_stillborn(infant_monkey)			\
			: monkey_lives						\
		})()

#define 	give_birth_to_monkey()						\
										\
		LAMBDA(bool,(void) {						\
			bool living ;						\
										\
			living = true ;						\
										\
			birth_of(infant_monkey) ;				\
			if (!living) goto afterbirth ;				\
			birth_of(infant_monkey->cube) ;				\
										\
afterbirth:									\
			return !living ; 					\
		})()								

//TODO
Monkey * monkey_birth() {
	Monkey * infant_monkey ;
	bool is_stillborn ;

	is_stillborn = give_birth_to_monkey() ;
	
	if(is_stillborn)
		infant_monkey = NULL ;

	return infant_monkey ;
}

#define 	free_if_valid(ptr)						\
										\
		LAMBDA(void,(void) {						\
			if(*ptr) { 						\
				free(*ptr) ; 					\
				*ptr = NULL ;					\
			}	 						\
		})()

#define 	generic_killer(ptr)						\
										\
		LAMBDA(void,(void) {						\
			free_if_valid(ptr) ;					\
		})()

#define 	monkey_killer(ptr)						\
										\
		LAMBDA(void,(void) {						\
			generic_killer(ptr) ;					\
		})()

#define         find_killer(id)							\
		_Generic( ((id)),						\
			Monkey *:	monkey_killer,	 			\
			RubixCube *:	rubix_cube_free, 			\
			default:	generic_killer				\
		)(id) ;								\

#define 	death_of(id)							\
										\
		LAMBDA(void,(void) {						\
			find_killer(id) ; 					\
		})()

#define 	capture_and_euthanize(target_monkey)				\
										\
		LAMBDA(void,(void) {						\
			rubix_cube_free((target_monkey)->cube) ;		\
			monkey_killer(&target_monkey) ;				\
		})()
			/* death_of((target_monkey)->cube) ;			\ */
			/* death_of((target_monkey)) ;				\ */


void monkey_death(Monkey ** target_monkey_location) {
	/* Ensure that the monkey is real */
	if (!target_monkey_location) return ;

	/* If it is, kill it. */
	capture_and_euthanize(*target_monkey_location) ;

	/* There is no longer a monkey at this location. */
	*target_monkey_location = NULL ;
}
