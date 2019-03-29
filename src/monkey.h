#ifndef MONKEY_H
#define MONKEY_H "monkey.h"

#include <pthread.h>
#include "rubix_cube.h"

/* begin monkey type section */

typedef enum monkey_role {
	MONKEY_ENTREPRENEUR, 	/* this one works 80+ hours per week	*/
	MONKEY_MANGER,		/* this one works close to 60 		*/
	MONKEY_WORKER, 		/* a purely 9-5 kind of monkey 		*/
	MONKEY_UNEMPLOYED	/* this one is in an error state 	*/
} MonkeyRole ;

typedef struct monkey_staff MonkeyStaff ;

typedef struct monkey {
	RubixCube * 			cube ;
	pthread_mutex_t			cube_mutex ;
	RubixCubeScramble * 		history ;
	MonkeyStaff * 			staff ;
} Monkey ;

typedef struct monkey_staff {
	Monkey *			personnel ;
	size_t				headcount ;
} MonkeyStaff ;

/* end monkey type section */

/* begin monkey manipulation section */

Monkey * 	monkey_birth		() ;
void		monkey_death		(Monkey ** target_monkey_location) ;

/* end monkey manipulation section */

#endif /* ifndef MONKEY_H */
