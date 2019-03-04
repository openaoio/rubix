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
 " Rubix cube\n"
 "(behind) +--+--+--+\n"
 "  D    / %c / %c / %c /|\n"
 "  ->  +--+--+--+ |\n"
 "     / %c / %c/  %c/| %c|	\n"
 "    +--+--+--+ | %c|	\n"
 "   / %c/ %c/ %c/| |/| <-- E (behind)\n"
 "  +--+--+--+ |/%c| %c|%c \n"
 "  | %c| %c| %c|/|%c|/|      	\n"
 "  +--+--+--+ |/| %c|%c     2  \n"
 "  | %c| %c| %c|%c/| %c|/           _.\n"
 "  +--+--+--+ |/%c       1    /|\n"
 "  | %c| %c| %c| /             /  \n"
 "  +--+--+--+^         0   / Direction of array of rubix cube face arrays\n"
 "       F ^\n"
 " (behind)|\n"
 "/\n" ;

void rubix_cube_print_ascii(FILE * output_file, RubixCube * pRubix_cube) {
	// TODO:
	//fprintf(output_file,rubix_cube_whole_cube_format_string,

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

void rubix_cube_print_face_ascii(FILE * output_file, RubixCube * pRubix_cube, RubixCubeSide side) {
	RubixCubeFace face = rubix_cube_get_face_of_side(pRubix_cube,side) ;
	fprintf(output_file,rubix_cube_face_format_string,
		RUBIX_CUBE_FACE_TO_PRINTF_ARGS(face) ) ;
}
