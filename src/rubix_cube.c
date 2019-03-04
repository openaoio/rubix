#include "rubix_cube.h"

const char RUBIX_CUBE_COLOR_CHARS[] = {
	'X', /* NULL */
	'W', /* WHITE */
	'R', /* RED */
	'B', /* BLUE */
	'G', /* GREEN */
	'O', /* ORANGE */
	'Y'  /* YELLOW */
} ;

RubixCube rubix_cube_generate_solved(void) {
	return RUBIX_CUBE_SOLVED_LITERAL ;	
}

#define TTT "whatup"

struct a {
	int x ;
	int y ;
} ;

struct a hello() {
	printf("hey %d \n",RUBIX_CUBE_COLOR_WHITE) ;
	struct a x = { .x = 5, .y = 6 } ;
	return x ;
}


RubixCube rubix_cube_generate_scrambled(rubix_cube_seed_t seed) {
	// TODO: stub, will shuffle
	return RUBIX_CUBE_SOLVED_LITERAL ;	
}

char rubix_cube_get_char_of_color(RubixCubeColor color) {
	return RUBIX_CUBE_COLOR_CHARS[(size_t)color] ;
}

RubixCubeFace rubix_cube_get_face_of_side(RubixCube * pRubix_cube, RubixCubeSide side) {
	// TODO:  all sides
	switch(side) {
		case RUBIX_CUBE_SQUARE_FRONT:
			return RUBIX_CUBE_GET_FRONT_FACE((*pRubix_cube)) ;
		case RUBIX_CUBE_SQUARE_TOP:
			return RUBIX_CUBE_GET_TOP_FACE((*pRubix_cube)) ;
		case RUBIX_CUBE_SQUARE_RIGHT:
			return RUBIX_CUBE_GET_RIGHT_FACE((*pRubix_cube)) ;
		case RUBIX_CUBE_SQUARE_LEFT:
			return RUBIX_CUBE_GET_LEFT_FACE((*pRubix_cube)) ;
		case RUBIX_CUBE_SQUARE_BACK:
			return RUBIX_CUBE_GET_BACK_FACE((*pRubix_cube)) ;
		case RUBIX_CUBE_SQUARE_BOTTOM:
			return RUBIX_CUBE_GET_BOTTOM_FACE((*pRubix_cube)) ;
		default:
			return RUBIX_CUBE_GET_INVALID_FACE((*pRubix_cube)) ;
	}
}


const char rubix_cube_face_format_string[] =
 "+---+---+---+\n"
 "| %c | %c | %c |\n"
 "+---+---+---+\n"
 "| %c | %c | %c |\n"
 "+---+---+---+\n"
 "| %c | %c | %c |\n"
 "+---+---+---+\n" ;

const char rubix_cube_whole_cube_format_string[] =
 " ====Rubix cube====\n"
 "        +--+--+--+\n"
 "  TOP  / %c/ %c/ %c/ |\n"
 "      +--+--+--+  |\n"
 "     / %c/ %c/ %c/|  |\n"
 "    +--+--+--+ |%c |	\n"
 "   / %c/ %c/ %c/|%c|/ |   RIGHT\n"
 "  +--+--+--+%c|/|%c/| \n"
 "  | %c| %c| %c|/|%c|/ |      	\n"
 "  +--+--+--+%c|/|%c |     2  \n"
 "  | %c| %c| %c|/|%c|/           _.\n"
 "  +--+--+--+%c|/       1     /|\n"
 "  | %c| %c| %c| /             /  \n"
 "  +--+--+--+^         0   / Direction of array of rubix cube face arrays\n"
 "      FRONT \n"
 " \n"
 " \n" ;

void rubix_cube_print_ascii(FILE * output_file, RubixCube * pRubix_cube) {

	RubixCubeFace top_face = rubix_cube_get_face_of_side(pRubix_cube,RUBIX_CUBE_SQUARE_TOP) ;
	RubixCubeFace front_face = rubix_cube_get_face_of_side(pRubix_cube,RUBIX_CUBE_SQUARE_FRONT) ;
	RubixCubeFace right_face = rubix_cube_get_face_of_side(pRubix_cube,RUBIX_CUBE_SQUARE_RIGHT) ;

	printf("hello\n") ;

	fprintf(output_file,rubix_cube_whole_cube_format_string,
		rubix_cube_get_char_of_color(top_face.squares[0]),
		rubix_cube_get_char_of_color(top_face.squares[1]),
		rubix_cube_get_char_of_color(top_face.squares[2]),
		rubix_cube_get_char_of_color(top_face.squares[3]),
		rubix_cube_get_char_of_color(top_face.squares[4]),
		rubix_cube_get_char_of_color(top_face.squares[6]),
		rubix_cube_get_char_of_color(right_face.squares[2]),
		rubix_cube_get_char_of_color(top_face.squares[6]),
		rubix_cube_get_char_of_color(top_face.squares[7]),
		rubix_cube_get_char_of_color(top_face.squares[8]),
		rubix_cube_get_char_of_color(right_face.squares[1]),
		rubix_cube_get_char_of_color(right_face.squares[0]),
		rubix_cube_get_char_of_color(right_face.squares[5]),
		rubix_cube_get_char_of_color(front_face.squares[0]),
		rubix_cube_get_char_of_color(front_face.squares[1]),
		rubix_cube_get_char_of_color(front_face.squares[2]),
		rubix_cube_get_char_of_color(right_face.squares[4]),
		rubix_cube_get_char_of_color(right_face.squares[3]),
		rubix_cube_get_char_of_color(right_face.squares[8]),
		rubix_cube_get_char_of_color(front_face.squares[3]),
		rubix_cube_get_char_of_color(front_face.squares[4]),
		rubix_cube_get_char_of_color(front_face.squares[5]),
		rubix_cube_get_char_of_color(right_face.squares[7]),
		rubix_cube_get_char_of_color(right_face.squares[6]),
		rubix_cube_get_char_of_color(front_face.squares[6]),
		rubix_cube_get_char_of_color(front_face.squares[7]),
		rubix_cube_get_char_of_color(front_face.squares[8])
	) ;

}

#define RUBIX_CUBE_FACE_TO_PRINTF_ARGS(rubix_cube_face_id) \
	rubix_cube_get_char_of_color(rubix_cube_face_id.squares[0]), \
	rubix_cube_get_char_of_color(rubix_cube_face_id.squares[1]), \
	rubix_cube_get_char_of_color(rubix_cube_face_id.squares[2]), \
	rubix_cube_get_char_of_color(rubix_cube_face_id.squares[3]), \
	rubix_cube_get_char_of_color(rubix_cube_face_id.squares[4]), \
	rubix_cube_get_char_of_color(rubix_cube_face_id.squares[5]), \
	rubix_cube_get_char_of_color(rubix_cube_face_id.squares[6]), \
	rubix_cube_get_char_of_color(rubix_cube_face_id.squares[7]), \
	rubix_cube_get_char_of_color(rubix_cube_face_id.squares[8])

void rubix_cube_print_all_faces_ascii(FILE * output_file, RubixCube * pRubix_cube) {
	printf("Top face:\n") ;
	rubix_cube_print_face_ascii(stdout,pRubix_cube,RUBIX_CUBE_SQUARE_TOP) ;
	printf("Front face:\n") ;
	rubix_cube_print_face_ascii(stdout,pRubix_cube,RUBIX_CUBE_SQUARE_FRONT) ;
	printf("Right face:\n") ;
	rubix_cube_print_face_ascii(stdout,pRubix_cube,RUBIX_CUBE_SQUARE_RIGHT) ;
	printf("Left face:\n") ;
	rubix_cube_print_face_ascii(stdout,pRubix_cube,RUBIX_CUBE_SQUARE_LEFT) ;
	printf("Back face:\n") ;
	rubix_cube_print_face_ascii(stdout,pRubix_cube,RUBIX_CUBE_SQUARE_BACK) ;
	printf("Bottom face:\n") ;
	rubix_cube_print_face_ascii(stdout,pRubix_cube,RUBIX_CUBE_SQUARE_BOTTOM) ;
}

void rubix_cube_print_face_ascii(FILE * output_file, RubixCube * pRubix_cube, RubixCubeSide side) {
	RubixCubeFace face = rubix_cube_get_face_of_side(pRubix_cube,side) ;
	fprintf(output_file,rubix_cube_face_format_string,
		RUBIX_CUBE_FACE_TO_PRINTF_ARGS(face) ) ;
}

void makeprint() {
	RubixCube test = rubix_cube_generate_solved() ;
	rubix_cube_print_all_faces_ascii(stdout,&test) ;
	rubix_cube_print_ascii(stdout,&test) ;
}

