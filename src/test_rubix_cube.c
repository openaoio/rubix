#include "rubix_cube.h"
#include "../lil_test/src/lil_test.h"

TEST_SET(create_destroy,
	TEST_CASE(on_stack,
		RubixCube test_cube = rubix_cube_generate_solved() ;
		ASSERT(rubix_cube_is_solved(&test_cube)) ;
	)
	TEST_CASE(on_heap,
		RubixCube * pTest_cube = (RubixCube*)malloc(sizeof(RubixCube)) ;
		*pTest_cube = rubix_cube_generate_solved() ;
		free(pTest_cube) ;
	)
	TEST_CASE(on_heap_native,
		RubixCube * pRubix_cube = rubix_cube_allocate_solved() ;
		rubix_cube_free(pRubix_cube) ;
	) ;
)

/* TEST_SET(printing, */
/* 	RubixCube solved_cube = rubix_cube_generate_solved() ; */

/* 	TEST_CASE(print_front, */
/* 		printf("Top face:\n") ; */
/* 		rubix_cube_print_face_ascii(stdout,&solved_cube,RUBIX_CUBE_SQUARE_TOP) ; */
/* 		printf("Front face:\n") ; */
/* 		rubix_cube_print_face_ascii(stdout,&solved_cube,RUBIX_CUBE_SQUARE_FRONT) ; */
/* 		printf("Right face:\n") ; */
/* 		rubix_cube_print_face_ascii(stdout,&solved_cube,RUBIX_CUBE_SQUARE_RIGHT) ; */
/* 		printf("Left face:\n") ; */
/* 		rubix_cube_print_face_ascii(stdout,&solved_cube,RUBIX_CUBE_SQUARE_LEFT) ; */
/* 		printf("Back face:\n") ; */
/* 		rubix_cube_print_face_ascii(stdout,&solved_cube,RUBIX_CUBE_SQUARE_BACK) ; */
/* 		printf("Bottom face:\n") ; */
/* 		rubix_cube_print_face_ascii(stdout,&solved_cube,RUBIX_CUBE_SQUARE_BOTTOM) ; */
/* 	) */
/* ) */

TEST_SET(rotate_front,
	TEST_CASE(four_times_clockwise,
		RubixCube * test = rubix_cube_allocate_solved() ;	

		rubix_cube_rotate_front_face(test,RUBIX_CUBE_FACE_ROTATION_CLOCKWISE) ;
		ASSERT(!rubix_cube_is_solved(test)) ;

		rubix_cube_rotate_front_face(test,RUBIX_CUBE_FACE_ROTATION_CLOCKWISE) ;
		ASSERT(!rubix_cube_is_solved(test)) ;

		rubix_cube_rotate_front_face(test,RUBIX_CUBE_FACE_ROTATION_CLOCKWISE) ;
		ASSERT(!rubix_cube_is_solved(test)) ;

		rubix_cube_rotate_front_face(test,RUBIX_CUBE_FACE_ROTATION_CLOCKWISE) ;
		ASSERT(rubix_cube_is_solved(test)) ;
		rubix_cube_free(test) ;	
	) ;

	TEST_CASE(four_times_counterclockwise,
		RubixCube * test = rubix_cube_allocate_solved() ;	

		rubix_cube_rotate_front_face(test,RUBIX_CUBE_FACE_ROTATION_COUNTERCLOCKWISE) ;
		rubix_cube_rotate_front_face(test,RUBIX_CUBE_FACE_ROTATION_COUNTERCLOCKWISE) ;
		ASSERT(!rubix_cube_is_solved(test)) ;

		rubix_cube_rotate_front_face(test,RUBIX_CUBE_FACE_ROTATION_COUNTERCLOCKWISE) ;
		rubix_cube_rotate_front_face(test,RUBIX_CUBE_FACE_ROTATION_COUNTERCLOCKWISE) ;
		ASSERT(rubix_cube_is_solved(test)) ;

		rubix_cube_free(test) ;	
	) ;

	TEST_CASE(twice_double,
		RubixCube * test = rubix_cube_allocate_solved() ;	

		rubix_cube_rotate_front_face(test,RUBIX_CUBE_FACE_ROTATION_DOUBLE) ;
		ASSERT(!rubix_cube_is_solved(test)) ;

		rubix_cube_rotate_front_face(test,RUBIX_CUBE_FACE_ROTATION_DOUBLE) ;
		ASSERT(rubix_cube_is_solved(test)) ;

		rubix_cube_free(test) ;	
	) ;
	
) ;


TEST_SET(is_solved,
	TEST_CASE(id_and_id,
		RubixCube solved = rubix_cube_generate_solved() ;
		ASSERT(rubix_cube_is_solved(&solved)) ;
	) ;
) ;

TEST_SET(rotate_top,
	TEST_CASE(four_times_clockwise,
		RubixCube * test = rubix_cube_allocate_solved() ;	

		rubix_cube_rotate_top_face(test,RUBIX_CUBE_FACE_ROTATION_CLOCKWISE) ;
		rubix_cube_rotate_top_face(test,RUBIX_CUBE_FACE_ROTATION_CLOCKWISE) ;
		ASSERT(!rubix_cube_is_solved(test)) ;

		rubix_cube_rotate_top_face(test,RUBIX_CUBE_FACE_ROTATION_CLOCKWISE) ;
		rubix_cube_rotate_top_face(test,RUBIX_CUBE_FACE_ROTATION_CLOCKWISE) ;
		ASSERT(rubix_cube_is_solved(test)) ;

		rubix_cube_free(test) ;	
	) ;

	TEST_CASE(four_times_counterclockwise,
		RubixCube * test = rubix_cube_allocate_solved() ;	

		rubix_cube_rotate_top_face(test,RUBIX_CUBE_FACE_ROTATION_COUNTERCLOCKWISE) ;
		rubix_cube_rotate_top_face(test,RUBIX_CUBE_FACE_ROTATION_COUNTERCLOCKWISE) ;
		ASSERT(!rubix_cube_is_solved(test)) ;

		rubix_cube_rotate_top_face(test,RUBIX_CUBE_FACE_ROTATION_COUNTERCLOCKWISE) ;
		rubix_cube_rotate_top_face(test,RUBIX_CUBE_FACE_ROTATION_COUNTERCLOCKWISE) ;
		ASSERT(rubix_cube_is_solved(test)) ;

		rubix_cube_free(test) ;	
	) ;

	TEST_CASE(twice_double,
		RubixCube * test = rubix_cube_allocate_solved() ;	

		rubix_cube_rotate_top_face(test,RUBIX_CUBE_FACE_ROTATION_DOUBLE) ;
		ASSERT(!rubix_cube_is_solved(test)) ;

		rubix_cube_rotate_top_face(test,RUBIX_CUBE_FACE_ROTATION_DOUBLE) ;
		ASSERT(rubix_cube_is_solved(test)) ;

		rubix_cube_free(test) ;	
	) ;
) ;

TEST_SET(rotation_combos,
	TEST_CASE(top_front_undo,
		RubixCube * test = rubix_cube_allocate_solved() ;	
		for(unsigned i = 0 ; i < 4; ++i){
			rubix_cube_rotate_front_face(test,RUBIX_CUBE_FACE_ROTATION_DOUBLE) ;
			rubix_cube_rotate_top_face(test,RUBIX_CUBE_FACE_ROTATION_DOUBLE) ;
		}

		ASSERT(!rubix_cube_is_solved(test)) ;

		for(unsigned i = 0 ; i < 2; ++i){
			rubix_cube_rotate_front_face(test,RUBIX_CUBE_FACE_ROTATION_DOUBLE) ;
			rubix_cube_rotate_top_face(test,RUBIX_CUBE_FACE_ROTATION_DOUBLE) ;
		}

		ASSERT(rubix_cube_is_solved(test)) ;

		rubix_cube_free(test) ;	
	)
)

TEST_SET(generic_rotation,
	TEST_CASE(front_clock_counterclock,
		RubixCube cube = rubix_cube_generate_solved() ;
		rubix_cube_print_ascii_stdout(&cube) ;
		rubix_cube_rotate_face(&cube,RUBIX_CUBE_SQUARE_FRONT,RUBIX_CUBE_FACE_ROTATION_CLOCKWISE) ;
		ASSERT(!rubix_cube_is_solved(&cube)) ;
		rubix_cube_print_ascii_stdout(&cube) ;
		rubix_cube_rotate_face(&cube,RUBIX_CUBE_SQUARE_FRONT,RUBIX_CUBE_FACE_ROTATION_COUNTERCLOCKWISE ) ;
		rubix_cube_print_ascii_stdout(&cube) ;
		ASSERT(rubix_cube_is_solved(&cube)) ;
	)

	TEST_CASE(top_clock_counterclock,
		RubixCube cube = rubix_cube_generate_solved() ;
		rubix_cube_print_ascii_stdout(&cube) ;
		rubix_cube_rotate_face(&cube,RUBIX_CUBE_SQUARE_TOP,RUBIX_CUBE_FACE_ROTATION_CLOCKWISE) ;
		ASSERT(!rubix_cube_is_solved(&cube)) ;
		rubix_cube_print_ascii_stdout(&cube) ;
		rubix_cube_rotate_face(&cube,RUBIX_CUBE_SQUARE_TOP,RUBIX_CUBE_FACE_ROTATION_COUNTERCLOCKWISE ) ;
		rubix_cube_print_ascii_stdout(&cube) ;
		ASSERT(rubix_cube_is_solved(&cube)) ;
	)

	TEST_CASE(top_front_combo,
		RubixCube * test = rubix_cube_allocate_solved() ;	
		for(unsigned i = 0 ; i < 4; ++i){
			rubix_cube_rotate_face(test,RUBIX_CUBE_SQUARE_FRONT,RUBIX_CUBE_FACE_ROTATION_DOUBLE) ;
			rubix_cube_rotate_face(test,RUBIX_CUBE_SQUARE_TOP,RUBIX_CUBE_FACE_ROTATION_DOUBLE) ;
		}

		ASSERT(!rubix_cube_is_solved(test)) ;

		for(unsigned i = 0 ; i < 2; ++i){
			rubix_cube_rotate_face(test,RUBIX_CUBE_SQUARE_FRONT,RUBIX_CUBE_FACE_ROTATION_DOUBLE) ;
			rubix_cube_rotate_face(test,RUBIX_CUBE_SQUARE_TOP,RUBIX_CUBE_FACE_ROTATION_DOUBLE) ;
		}

		ASSERT(rubix_cube_is_solved(test)) ;

		rubix_cube_free(test) ;	
	)

	TEST_CASE(back_right_combo,
		RubixCube * test = rubix_cube_allocate_solved() ;	
		for(unsigned i = 0 ; i < 4; ++i){
			rubix_cube_rotate_face(test,RUBIX_CUBE_SQUARE_BACK,RUBIX_CUBE_FACE_ROTATION_DOUBLE) ;
			rubix_cube_rotate_face(test,RUBIX_CUBE_SQUARE_RIGHT,RUBIX_CUBE_FACE_ROTATION_DOUBLE) ;
		}

		ASSERT(!rubix_cube_is_solved(test)) ;

		for(unsigned i = 0 ; i < 2; ++i){
			rubix_cube_rotate_face(test,RUBIX_CUBE_SQUARE_BACK,RUBIX_CUBE_FACE_ROTATION_DOUBLE) ;
			rubix_cube_rotate_face(test,RUBIX_CUBE_SQUARE_RIGHT,RUBIX_CUBE_FACE_ROTATION_DOUBLE) ;
		}

		ASSERT(rubix_cube_is_solved(test)) ;

		rubix_cube_free(test) ;	
	)
)
	

TEST_MAIN() ;

