#include "rubix_cube.h"
#include "/home/jsavitz/lil_test/src/lil_test.h"

// TODO: Submodle lil_test in this repo and link locally

TEST_SET(create_destroy,
	TEST_CASE(on_stack,
		RubixCube test_cube = rubix_cube_generate_solved() ;
		printf("first 8 bytes of rubix cube as hex: %llx\n",
			*(rubix_cube_seed_t *)&test_cube) ;
	)
	TEST_CASE(on_heap,
		RubixCube * pTest_cube = (RubixCube*)malloc(sizeof(RubixCube)) ;
		*pTest_cube = rubix_cube_generate_solved() ;
		free(pTest_cube) ;
	)
)

TEST_SET(printing,
	RubixCube solved_cube = rubix_cube_generate_solved() ;

	TEST_CASE(print_front,
		printf("Top face:\n") ;
		rubix_cube_print_face_ascii(stdout,&solved_cube,RUBIX_CUBE_SQUARE_TOP) ;
		printf("Front face:\n") ;
		rubix_cube_print_face_ascii(stdout,&solved_cube,RUBIX_CUBE_SQUARE_FRONT) ;
		printf("Right face:\n") ;
		rubix_cube_print_face_ascii(stdout,&solved_cube,RUBIX_CUBE_SQUARE_RIGHT) ;
		printf("Left face:\n") ;
		rubix_cube_print_face_ascii(stdout,&solved_cube,RUBIX_CUBE_SQUARE_LEFT) ;
		printf("Back face:\n") ;
		rubix_cube_print_face_ascii(stdout,&solved_cube,RUBIX_CUBE_SQUARE_BACK) ;
		printf("Bottom face:\n") ;
		rubix_cube_print_face_ascii(stdout,&solved_cube,RUBIX_CUBE_SQUARE_BOTTOM) ;
	)
)

TEST_MAIN() ;

