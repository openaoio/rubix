/*
    A simple rubix cube simulator.
    Copyright (C) 2019 Joel Savitz <joelsavitz@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    A copy of the GNU General Public License can be found in the file
    LICENSE, but if you prefer, see <https://www.gnu.org/licenses/>.
*/
#include "rubix_cube.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>

const char RUBIX_CUBE_COLOR_CHARS[] = {
	'X', /* NULL */
	'W', /* WHITE */
	'R', /* RED */
	'B', /* BLUE */
	'G', /* GREEN */
	'O', /* ORANGE */
	'Y'  /* YELLOW */
} ;

const char * RUBIX_CUBE_SIDE_STRINGS[] = {
	"Null",
	"Top",
	"Front",
	"Right",
	"Left",
	"Back",
	"Bottom"
} ;

const char * RUBIX_CUBE_FACE_ROTATION_STRINGS[] = {
	"Clockwise",
	"Counter-Clockwise",
	"Double"
} ;

size_t rubix_cube_global_count = 0 ;
RubixCube _rubix_cube_generate_solved_with_ID() {
	RubixCube new = RUBIX_CUBE_SOLVED_LITERAL ;
	new.ID = rubix_cube_global_count++ ;
	return new ;
}

RubixCube rubix_cube_generate_solved(void) {
	return _rubix_cube_generate_solved_with_ID() ;
}

RubixCube rubix_cube_generate_scrambled(RubixCubeSeed seed) {
	RubixCube new = rubix_cube_generate_solved() ;
	RubixCubeMove moves[RUBIX_CUBE_SCRAMBLE_INTENSITY] ;
	rubix_cube_generate_moves_from_seed(seed,RUBIX_CUBE_SCRAMBLE_INTENSITY,moves) ;
	
	for(size_t i = 0; i < RUBIX_CUBE_SCRAMBLE_INTENSITY; ++i) {
		rubix_cube_apply_move(&new,&moves[i]) ;
	}

	return new ;

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

const char rubix_cube_half_cube_format_string[] =
 " ====Rubix cube====\n"
 "        +--+--+--+\n"
 "  TOP  / %c/ %c/ %c/|\n"
 "      +--+--+--+ |\n"
 "     / %c/ %c/ %c/| |\n"
 "    +--+--+--+ |%c|	\n"
 "   / %c/ %c/ %c/|%c|/|   RIGHT\n"
 "  +--+--+--+%c|/|%c/ \n"
 "  | %c| %c| %c|/|%c|/|      	\n"
 "  +--+--+--+%c|/|%c|      2 \n"
 "  | %c| %c| %c|/|%c|/           _.\n"
 "  +--+--+--+%c|/        1    /|\n"
 "  | %c| %c| %c| /             /  \n"
 "  +--+--+--+^         0   / Direction of array of rubix cube face arrays\n"
 "      FRONT \n"
 " \n"
 " \n" ;

void rubix_cube_print_ascii(FILE * output_file, RubixCube * pRubix_cube) {

	RubixCubeFace top_face = rubix_cube_get_face_of_side(pRubix_cube,RUBIX_CUBE_SQUARE_TOP) ;
	RubixCubeFace front_face = rubix_cube_get_face_of_side(pRubix_cube,RUBIX_CUBE_SQUARE_FRONT) ;
	RubixCubeFace right_face = rubix_cube_get_face_of_side(pRubix_cube,RUBIX_CUBE_SQUARE_RIGHT) ;

	fprintf(output_file,rubix_cube_half_cube_format_string,
		/* begin top face */
		rubix_cube_get_char_of_color(top_face.squares[0]),
		rubix_cube_get_char_of_color(top_face.squares[1]),
		rubix_cube_get_char_of_color(top_face.squares[2]),
		rubix_cube_get_char_of_color(top_face.squares[3]),
		rubix_cube_get_char_of_color(top_face.squares[4]),
		rubix_cube_get_char_of_color(top_face.squares[5]),
		/* begin right face (mixed in) */
		rubix_cube_get_char_of_color(right_face.squares[2]),
		rubix_cube_get_char_of_color(top_face.squares[6]),
		rubix_cube_get_char_of_color(top_face.squares[7]),
		rubix_cube_get_char_of_color(top_face.squares[8]),
		rubix_cube_get_char_of_color(right_face.squares[1]),
		rubix_cube_get_char_of_color(right_face.squares[0]),
		rubix_cube_get_char_of_color(right_face.squares[5]),
		/* begin front face */
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

const char rubix_cube_whole_cube_format_string[] =
 " ==========================[The Rubix Cube]==========================\n"
 "        +--+--+--+                       +--+--+--+                  \n"
 "  TOP  / %c/ %c/ %c/|            BOTTOM    / %c/ %c/ %c/|            \n"
 "      +--+--+--+ |                     +--+--+--+ |                  \n"
 "     / %c/ %c/ %c/| |                    / %c/ %c/ %c/| |            \n"
 "    +--+--+--+ |%c|	             +--+--+--+ |%c|	               \n"
 "   / %c/ %c/ %c/|%c|/|   RIGHT          / %c/ %c/ %c/|%c|/|   BACK   \n"
 "  +--+--+--+%c|/|%c/                 +--+--+--+%c|/|%c/              \n"
 "  | %c| %c| %c|/|%c|/|      	   | %c| %c| %c|/|%c|/|      	       \n"
 "  +--+--+--+%c|/|%c|      2          +--+--+--+%c|/|%c|              \n"
 "  | %c| %c| %c|/|%c|/           _.     | %c| %c| %c|/|%c|/           \n"
 "  +--+--+--+%c|/        1    /|     +--+--+--+%c|/                   \n"
 "  | %c| %c| %c| /             /       | %c| %c| %c| /                \n"
 "  +--+--+--+^         0   /        +--+--+--+^        0  1  2        \n"
 "      FRONT                              LEFT       ------->         \n"
 "                                                                     \n"
 " ====================================================================\n"
 "                                                                     \n" ;

void rubix_cube_print_ascii_double(FILE * output_file, RubixCube * pRubix_cube) {

	RubixCubeFace top_face = rubix_cube_get_face_of_side(pRubix_cube,RUBIX_CUBE_SQUARE_TOP) ;
	RubixCubeFace front_face = rubix_cube_get_face_of_side(pRubix_cube,RUBIX_CUBE_SQUARE_FRONT) ;
	RubixCubeFace right_face = rubix_cube_get_face_of_side(pRubix_cube,RUBIX_CUBE_SQUARE_RIGHT) ;
	RubixCubeFace left_face = rubix_cube_get_face_of_side(pRubix_cube,RUBIX_CUBE_SQUARE_LEFT) ;
	RubixCubeFace back_face = rubix_cube_get_face_of_side(pRubix_cube,RUBIX_CUBE_SQUARE_BACK) ;
	RubixCubeFace bottom_face = rubix_cube_get_face_of_side(pRubix_cube,RUBIX_CUBE_SQUARE_BOTTOM) ;

	(void)back_face, (void)left_face ;

	fprintf(output_file,rubix_cube_whole_cube_format_string,
		/* LINE 0 */
		/* begin top face */
		rubix_cube_get_char_of_color(top_face.squares[0]),
		rubix_cube_get_char_of_color(top_face.squares[1]),
		rubix_cube_get_char_of_color(top_face.squares[2]),

		/* begin bottom face */
		rubix_cube_get_char_of_color(bottom_face.squares[2]),
		rubix_cube_get_char_of_color(bottom_face.squares[5]),
		rubix_cube_get_char_of_color(bottom_face.squares[8]),
		/* LINE 1 */

		rubix_cube_get_char_of_color(top_face.squares[3]),
		rubix_cube_get_char_of_color(top_face.squares[4]),
		rubix_cube_get_char_of_color(top_face.squares[5]),

		rubix_cube_get_char_of_color(bottom_face.squares[1]),
		rubix_cube_get_char_of_color(bottom_face.squares[4]),
		rubix_cube_get_char_of_color(bottom_face.squares[7]),

		/* LINE 2 */
		/* begin right & left face (mixed in) */
		rubix_cube_get_char_of_color(right_face.squares[2]),
		rubix_cube_get_char_of_color(back_face.squares[6]),

		/* LINE 3 */
		rubix_cube_get_char_of_color(top_face.squares[6]),
		rubix_cube_get_char_of_color(top_face.squares[7]),
		rubix_cube_get_char_of_color(top_face.squares[8]),
		rubix_cube_get_char_of_color(right_face.squares[1]),

		rubix_cube_get_char_of_color(bottom_face.squares[0]),
		rubix_cube_get_char_of_color(bottom_face.squares[3]),
		rubix_cube_get_char_of_color(bottom_face.squares[6]),
		rubix_cube_get_char_of_color(back_face.squares[1]),

		/* LINE 4 */
		rubix_cube_get_char_of_color(right_face.squares[0]),
		rubix_cube_get_char_of_color(right_face.squares[5]),

		rubix_cube_get_char_of_color(back_face.squares[3]),
		rubix_cube_get_char_of_color(back_face.squares[8]),

		/* LINE 5 */
		/* begin front face */
		rubix_cube_get_char_of_color(front_face.squares[0]),
		rubix_cube_get_char_of_color(front_face.squares[1]),
		rubix_cube_get_char_of_color(front_face.squares[2]),
		rubix_cube_get_char_of_color(right_face.squares[4]),

		rubix_cube_get_char_of_color(left_face.squares[8]),
		rubix_cube_get_char_of_color(left_face.squares[7]),
		rubix_cube_get_char_of_color(left_face.squares[6]),
		rubix_cube_get_char_of_color(back_face.squares[4]),

		/* LINE 6 */
		rubix_cube_get_char_of_color(right_face.squares[3]),
		rubix_cube_get_char_of_color(right_face.squares[8]),

		rubix_cube_get_char_of_color(back_face.squares[0]),
		rubix_cube_get_char_of_color(back_face.squares[5]),

		/* LINE 7 */
		rubix_cube_get_char_of_color(front_face.squares[3]),
		rubix_cube_get_char_of_color(front_face.squares[4]),
		rubix_cube_get_char_of_color(front_face.squares[5]),
		rubix_cube_get_char_of_color(right_face.squares[7]),

		rubix_cube_get_char_of_color(left_face.squares[5]),
		rubix_cube_get_char_of_color(left_face.squares[4]),
		rubix_cube_get_char_of_color(left_face.squares[3]),
		rubix_cube_get_char_of_color(back_face.squares[5]),

		/* LINE 8 */
		rubix_cube_get_char_of_color(right_face.squares[6]),

		rubix_cube_get_char_of_color(back_face.squares[2]),
		
		/* LINE 9 */
		rubix_cube_get_char_of_color(front_face.squares[6]),
		rubix_cube_get_char_of_color(front_face.squares[7]),
		rubix_cube_get_char_of_color(front_face.squares[8]),

		rubix_cube_get_char_of_color(left_face.squares[2]),
		rubix_cube_get_char_of_color(left_face.squares[1]),
		rubix_cube_get_char_of_color(left_face.squares[0])

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

RubixCube * rubix_cube_allocate_solved() {
	RubixCube * new_cube = (RubixCube*)malloc(sizeof(RubixCube)) ;
	if (!new_cube) {
		printf("RubixCube allocation failed!\n") ;
	} else {
		*new_cube = rubix_cube_generate_solved() ;
	}
	return new_cube ;
}

RubixCube * rubix_cube_allocate_scrambled(RubixCubeSeed seed) {

	RubixCube * new_cube = rubix_cube_allocate_solved() ;

	if (new_cube) *new_cube = rubix_cube_generate_scrambled(seed) ;

	return new_cube ;
}

void rubix_cube_free(RubixCube * pRubix_cube) {
	free(pRubix_cube) ;
}

void rubix_cube_print_ascii_stdout(RubixCube * pRubix_cube) {
	if (!pRubix_cube) return ;
	rubix_cube_print_ascii(stdout,pRubix_cube) ;
}

void rubix_cube_print_piece(RubixCubePiece piece) {
	printf(
		"TOP:\t%c\n"
		"FRONT:\t%c\n"
		"RIGHT:\t%c\n"
		"LEFT:\t%c\n"
		"BACK:\t%c\n"
		"BOTTOM:\t%c\n\n",
		rubix_cube_get_char_of_color(piece.sides[0]),
		rubix_cube_get_char_of_color(piece.sides[1]),
		rubix_cube_get_char_of_color(piece.sides[2]),
		rubix_cube_get_char_of_color(piece.sides[3]),
		rubix_cube_get_char_of_color(piece.sides[4]),
		rubix_cube_get_char_of_color(piece.sides[5])
	      ) ;
}

void swap_colors(RubixCubeColor * first, RubixCubeColor * second) {
	if (*first != *second) {
		*first ^= *second ;
		*second ^= *first ;
		*first ^= *second ;
	}
}

void swap_pieces(RubixCubePiece * first, RubixCubePiece * second) {
	/* printf("swapping piece:\n") ; */
	/* rubix_cube_print_piece(*first) ; */
	/* printf("and piece:\n") ; */
	/* rubix_cube_print_piece(*second) ; */
	/* printf("---\n") ; */
	if (first != second) {
		for(unsigned i = 0; i < RUBIX_CUBE_SIDE_COUNT; ++i) {
			/* printf("swapping...\n") ; */
			first->sides[i]  ^= second->sides[i] ;
			second->sides[i] ^= first->sides[i]  ;
			first->sides[i]  ^= second->sides[i] ;
		}
	}
		/* printf("done...\n") ; */
	/* printf("first piece:\n") ; */
	/* rubix_cube_print_piece(*first) ; */
	/* printf("second piece:\n") ; */
	/* rubix_cube_print_piece(*second) ; */
	/* printf("---\n") ; */
}

void rubix_cube_invert_piece(RubixCubePiece * piece, RubixCubeRotation rotation) {
	// TODO: stub
	switch(rotation) {
		case RUBIX_CUBE_ROTATE_UP:
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_TOP], &piece->sides[RUBIX_CUBE_SQUARE_BACK]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_BACK], &piece->sides[RUBIX_CUBE_SQUARE_BOTTOM]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_BOTTOM], &piece->sides[RUBIX_CUBE_SQUARE_FRONT]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_FRONT], &piece->sides[RUBIX_CUBE_SQUARE_TOP]) ;
			break ;
		case RUBIX_CUBE_ROTATE_DOWN:
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_TOP], &piece->sides[RUBIX_CUBE_SQUARE_FRONT]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_FRONT], &piece->sides[RUBIX_CUBE_SQUARE_BOTTOM]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_BOTTOM], &piece->sides[RUBIX_CUBE_SQUARE_BACK]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_BACK], &piece->sides[RUBIX_CUBE_SQUARE_TOP]) ;
			break ;
		case RUBIX_CUBE_ROTATE_RIGHT:
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_FRONT], &piece->sides[RUBIX_CUBE_SQUARE_RIGHT]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_RIGHT], &piece->sides[RUBIX_CUBE_SQUARE_BACK]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_BACK], &piece->sides[RUBIX_CUBE_SQUARE_LEFT]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_LEFT], &piece->sides[RUBIX_CUBE_SQUARE_FRONT]) ;
			break ;
		case RUBIX_CUBE_ROTATE_LEFT:
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_FRONT], &piece->sides[RUBIX_CUBE_SQUARE_LEFT]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_LEFT], &piece->sides[RUBIX_CUBE_SQUARE_BACK]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_BACK], &piece->sides[RUBIX_CUBE_SQUARE_RIGHT]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_RIGHT], &piece->sides[RUBIX_CUBE_SQUARE_FRONT]) ;
			break ;
		case RUBIX_CUBE_ROTATE_CLOCKWISE:
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_TOP], &piece->sides[RUBIX_CUBE_SQUARE_RIGHT]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_RIGHT], &piece->sides[RUBIX_CUBE_SQUARE_BOTTOM]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_BOTTOM], &piece->sides[RUBIX_CUBE_SQUARE_LEFT]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_LEFT], &piece->sides[RUBIX_CUBE_SQUARE_TOP]) ;
			break ;
		case RUBIX_CUBE_ROTATE_COUNTERCLOCKWISE:
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_TOP], &piece->sides[RUBIX_CUBE_SQUARE_RIGHT]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_RIGHT], &piece->sides[RUBIX_CUBE_SQUARE_BOTTOM]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_BOTTOM], &piece->sides[RUBIX_CUBE_SQUARE_LEFT]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_LEFT], &piece->sides[RUBIX_CUBE_SQUARE_TOP]) ;
			break ;
	}
}

void rubix_cube_rotate_piece(RubixCubePiece * piece, RubixCubeRotation rotation) {

	/* printf("in piece rotation:\n") ; */
	/* rubix_cube_print_piece(*piece) ; */
	/* printf("---\n") ; */
	switch(rotation) {
		case RUBIX_CUBE_ROTATE_UP:
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_TOP], &piece->sides[RUBIX_CUBE_SQUARE_BACK]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_TOP], &piece->sides[RUBIX_CUBE_SQUARE_BOTTOM]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_TOP], &piece->sides[RUBIX_CUBE_SQUARE_FRONT]) ;
			break ;
		case RUBIX_CUBE_ROTATE_DOWN:
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_TOP], &piece->sides[RUBIX_CUBE_SQUARE_FRONT]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_TOP], &piece->sides[RUBIX_CUBE_SQUARE_BOTTOM]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_TOP], &piece->sides[RUBIX_CUBE_SQUARE_BACK]) ;
			break ;
		case RUBIX_CUBE_ROTATE_RIGHT:
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_FRONT], &piece->sides[RUBIX_CUBE_SQUARE_RIGHT]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_FRONT], &piece->sides[RUBIX_CUBE_SQUARE_BACK]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_FRONT], &piece->sides[RUBIX_CUBE_SQUARE_LEFT]) ;
			break ;
		case RUBIX_CUBE_ROTATE_LEFT:
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_FRONT], &piece->sides[RUBIX_CUBE_SQUARE_LEFT]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_FRONT], &piece->sides[RUBIX_CUBE_SQUARE_BACK]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_FRONT], &piece->sides[RUBIX_CUBE_SQUARE_RIGHT]) ;
			break ;
		case RUBIX_CUBE_ROTATE_CLOCKWISE:
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_TOP], &piece->sides[RUBIX_CUBE_SQUARE_RIGHT]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_TOP], &piece->sides[RUBIX_CUBE_SQUARE_BOTTOM]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_TOP], &piece->sides[RUBIX_CUBE_SQUARE_LEFT]) ;
			break ;
		case RUBIX_CUBE_ROTATE_COUNTERCLOCKWISE:
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_TOP], &piece->sides[RUBIX_CUBE_SQUARE_LEFT]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_TOP], &piece->sides[RUBIX_CUBE_SQUARE_BOTTOM]) ;
			swap_colors(&piece->sides[RUBIX_CUBE_SQUARE_TOP], &piece->sides[RUBIX_CUBE_SQUARE_RIGHT]) ;
			break ;
	}
	/* rubix_cube_print_piece(*piece) ; */
	/* printf("---\n") ; */
}

/*
 * Note: This function operates on model with a modifed cube perspective
 *
 * A quadset is a set of four pieces.
 * They are passed to rubix_cube_rotate_piece_quadset() via @piece_quadset.
 *
 * They can be visualized as such:
 *
 * +---+---+
 * | 0 | 1 | This function applies rotations to a set of RubixCubePieces arranged
 * +---+---+ in this spatial configuration. Imagine rotating this square on a plane.
 * | 2 | 3 |
 * +---+---+
 *
 * There are three rotation types that can be done to this set:
 *
 * RUBIX_CUBE_ROTATE_QUADSET_RIGHT:	90 degree (aka  pi/2 radian) rotation
 * RUBIX_CUBE_ROTATE_QUADSET_LEFT:     -90 degree (aka -pi/2 radian) rotation
 * RUBIX_CUBE_ROTATE_QUADSET_FULL:     180 degree (aka  pi   radian) rotation
 *
 * These are defined using the RubixCubeRotateQuadset enumerated type in rubix_cube.h
 * the option is supplied to the function via @set_rotation
 *
 * The appropriate piece rotations are performed, supplied via @piece_rotation
 *
 */

void rubix_cube_rotate_piece_quadset(RubixCubePiece * piece_quadset[], RubixCubeRotationQuadset set_rotation, RubixCubeRotation piece_rotation) {
	int repititions = 0 ;
	/* for(int i = 0; i < 4; i++) { */
	/* 	rubix_cube_print_piece(*piece_quadset[i]) ; */
	/* 	printf("---\n") ; */
	/* } */
	switch(set_rotation) {
		case RUBIX_CUBE_ROTATE_QUADSET_RIGHT:
			swap_pieces(piece_quadset[0],piece_quadset[1]) ;
			swap_pieces(piece_quadset[0],piece_quadset[3]) ;
			swap_pieces(piece_quadset[0],piece_quadset[2]) ;
			break ;
		case RUBIX_CUBE_ROTATE_QUADSET_LEFT:
			swap_pieces(piece_quadset[0],piece_quadset[2]) ;
			swap_pieces(piece_quadset[0],piece_quadset[3]) ;
			swap_pieces(piece_quadset[0],piece_quadset[1]) ;
			break ;
		case RUBIX_CUBE_ROTATE_QUADSET_FULL:
			swap_pieces(piece_quadset[0],piece_quadset[3]) ;
			swap_pieces(piece_quadset[1],piece_quadset[2]) ;
			--repititions ;
			break ;
	}
	/* printf("post quadset rotate:\n") ; */
	/* for(int i = 0; i < 4; i++) { */
	/* 	rubix_cube_print_piece(*piece_quadset[i]) ; */
	/* 	printf("---\n") ; */
	/* } */

do_piece_rotation:
	for(unsigned i = 0; i < RUBIX_CUBE_PIECES_PER_QUADSET; ++i) {
		/* printf("executing piece rotation\n") ; */
		rubix_cube_rotate_piece(piece_quadset[i],piece_rotation) ;
	}
	if (!++repititions) goto do_piece_rotation ;
}

RubixCubePiece * firstpiece(RubixCube * pRubix_cube) {
	return &pRubix_cube->planes[0][0] ;	
}
RubixCubePiece * secondpiece(RubixCube * pRubix_cube) {
	return &pRubix_cube->planes[0][1] ;	
}

void rubix_cube_rotate_front_face(RubixCube * pRubix_cube, RubixCubeFaceRotation face_rotation) {
	RubixCubeRotation piece_rotation ;
	RubixCubeRotationQuadset set_rotation ;

	switch(face_rotation) {
		case RUBIX_CUBE_FACE_ROTATION_CLOCKWISE:
			piece_rotation = RUBIX_CUBE_ROTATE_CLOCKWISE ;
			set_rotation = RUBIX_CUBE_ROTATE_QUADSET_RIGHT ;
			break ;
		case RUBIX_CUBE_FACE_ROTATION_COUNTERCLOCKWISE:
			piece_rotation = RUBIX_CUBE_ROTATE_COUNTERCLOCKWISE ;
			set_rotation = RUBIX_CUBE_ROTATE_QUADSET_LEFT ;
			break ;
		case RUBIX_CUBE_FACE_ROTATION_DOUBLE:
			piece_rotation = RUBIX_CUBE_ROTATE_CLOCKWISE ;
			set_rotation = RUBIX_CUBE_ROTATE_QUADSET_FULL ;
			break ;
	}

	RubixCubePiece * corner_piece_quadset[RUBIX_CUBE_PIECES_PER_QUADSET] = {
			&pRubix_cube->planes[0][0],
			&pRubix_cube->planes[0][2],
			&pRubix_cube->planes[0][6],
			&pRubix_cube->planes[0][8]
	} ;
	rubix_cube_rotate_piece_quadset(corner_piece_quadset,set_rotation,piece_rotation) ;

	RubixCubePiece * side_piece_quadset[RUBIX_CUBE_PIECES_PER_QUADSET] = {
			&pRubix_cube->planes[0][1],
			&pRubix_cube->planes[0][5],
			&pRubix_cube->planes[0][3],
			&pRubix_cube->planes[0][7]
	} ;
	rubix_cube_rotate_piece_quadset(side_piece_quadset,set_rotation,piece_rotation) ;
}

void rubix_cube_rotate_top_face(RubixCube * pRubix_cube, RubixCubeFaceRotation face_rotation) {
	RubixCubeRotation piece_rotation ;
	RubixCubeRotationQuadset set_rotation ;

	switch(face_rotation) {
		case RUBIX_CUBE_FACE_ROTATION_CLOCKWISE:
			piece_rotation = RUBIX_CUBE_ROTATE_LEFT;
			set_rotation = RUBIX_CUBE_ROTATE_QUADSET_RIGHT ;
			break ;
		case RUBIX_CUBE_FACE_ROTATION_COUNTERCLOCKWISE:
			piece_rotation = RUBIX_CUBE_ROTATE_RIGHT ;
			set_rotation = RUBIX_CUBE_ROTATE_QUADSET_LEFT ;
			break ;
		case RUBIX_CUBE_FACE_ROTATION_DOUBLE:
			piece_rotation = RUBIX_CUBE_ROTATE_RIGHT ;
			set_rotation = RUBIX_CUBE_ROTATE_QUADSET_FULL ;
			break ;
	}

	RubixCubePiece * corner_piece_quadset[RUBIX_CUBE_PIECES_PER_QUADSET] = {
			&pRubix_cube->planes[2][0],
			&pRubix_cube->planes[2][2],
			&pRubix_cube->planes[0][0],
			&pRubix_cube->planes[0][2]
	} ;
	rubix_cube_rotate_piece_quadset(corner_piece_quadset,set_rotation,piece_rotation) ;

	RubixCubePiece * side_piece_quadset[RUBIX_CUBE_PIECES_PER_QUADSET] = {
			&pRubix_cube->planes[2][1],
			&pRubix_cube->planes[1][2],
			&pRubix_cube->planes[1][0],
			&pRubix_cube->planes[0][1]
	} ;
	rubix_cube_rotate_piece_quadset(side_piece_quadset,set_rotation,piece_rotation) ;
}


void rubix_cube_data_dump(RubixCube * pRubix_cube) {
	for (unsigned i = 0; i < RUBIX_CUBE_PLANE_COUNT; ++i) {
		printf("Plane %u:\n",i) ;
		for (unsigned j = 0; j < RUBIX_CUBE_PIECES_PER_PLANE; ++j) {
			printf("\tPiece %u:\n",j) ;
			for (unsigned k = 0; k < RUBIX_CUBE_SIDE_COUNT; ++k) {
				printf("\tSide %u color: %d\n",k,pRubix_cube->planes[i][j].sides[k]) ;
			
			}
		}
	}
}

void print_first(RubixCube * pRubix_cube) {

	/* rubix_cube_print_piece(*firstpiece(pRubix_cube)) ; */
	/* rubix_cube_rotate_piece(firstpiece(pRubix_cube),RUBIX_CUBE_ROTATE_DOWN) ; */
	/* printf("rotated down\n") ; */
	/* rubix_cube_print_piece(*firstpiece(pRubix_cube)) ; */

	/* rubix_cube_print_piece(*firstpiece(pRubix_cube)) ; */
	/* printf("---\n") ; */
	/* rubix_cube_print_piece(*secondpiece(pRubix_cube)) ; */
	/* printf("---\n") ; */

	/* swap_pieces(firstpiece(pRubix_cube),secondpiece(pRubix_cube)) ; */
	/* swap_pieces(firstpiece(pRubix_cube),secondpiece(pRubix_cube)) ; */

	/* rubix_cube_print_piece(*firstpiece(pRubix_cube)) ; */
	/* printf("---\n") ; */
	/* rubix_cube_print_piece(*secondpiece(pRubix_cube)) ; */
	/* printf("---\n") ; */
	//rubix_cube_rotate_front_face(pRubix_cube, RUBIX_CUBE_FACE_ROTATION_CLOCKWISE) ;
	//rubix_cube_data_dump(pRubix_cube) ;
	//
	swap_pieces(firstpiece(pRubix_cube),secondpiece(pRubix_cube)) ;


}

inline void rubix_cube_swap_pieces(RubixCubePiece * first, RubixCubePiece * second) {
	swap_pieces(first,second) ;
}


void rubix_cube_print_piece_from_cube(RubixCube * pRubix_cube, unsigned plane, unsigned index) {
	printf("Rubix cube piece @ [%u][%u]:\n",plane,index) ;
	rubix_cube_print_piece(pRubix_cube->planes[plane][index]) ;
}

int rubix_cube_equivelence_check(RubixCube * first, RubixCube * second) {
	/* If either arg is null, the cubes are not equal, but if they are both null, they are indeed equal */
	if (!first && !second) return 1 ;
	if (!first || !second) return 0 ;

	/* subtract number of non-color values /sizeof(RubixCubeColor) of to get length of color array */
	size_t color_values_per_cube = sizeof(RubixCube)/sizeof(RubixCubeColor) - sizeof(size_t)/sizeof(RubixCubeColor) ;
	RubixCubeColor *first_colors = (RubixCubeColor*)&first->planes;
	RubixCubeColor *second_colors = (RubixCubeColor*)&second->planes;


	for(size_t i = 0; i < color_values_per_cube; ++i) {
		if (first_colors[i] != second_colors[i]) return 0 ;
	}
	return 1 ;
}

/*
 * Compare a RubixCube object to the identity (intwise)
 * Will break early if early squares do not match
 */
inline int rubix_cube_is_solved(RubixCube * pRubix_cube) {
	return rubix_cube_equivelence_check(pRubix_cube,&RUBIX_CUBE_IDENTITY) ;
}


RubixCubeFaceRotationData rubix_cube_rotation_table[RUBIX_CUBE_SIDE_COUNT] = {
	/* TOP */
	{
		.corner_quadset = {
			{ 2, 0 },
			{ 2, 2 },
			{ 0, 0 },
			{ 0, 2 }

		},
		.side_quadset = {
			{ 2, 1 },
			{ 1, 2 },
			{ 1, 0 },
			{ 0, 1 }
		},
		.subrotation_set = {
			{ RUBIX_CUBE_ROTATE_QUADSET_RIGHT, RUBIX_CUBE_ROTATE_LEFT },
			{ RUBIX_CUBE_ROTATE_QUADSET_LEFT, RUBIX_CUBE_ROTATE_RIGHT },
			{ RUBIX_CUBE_ROTATE_QUADSET_FULL, RUBIX_CUBE_ROTATE_RIGHT }
		}
	},
	/* FRONT */
	{
		.corner_quadset = {
			{ 0, 0 },
			{ 0, 2 },
			{ 0, 6 },
			{ 0, 8 }

		},
		.side_quadset = {
			{ 0, 1 },
			{ 0, 5 },
			{ 0, 3 },
			{ 0, 7 }
		},
		.subrotation_set = {
			{ RUBIX_CUBE_ROTATE_QUADSET_RIGHT, RUBIX_CUBE_ROTATE_CLOCKWISE },
			{ RUBIX_CUBE_ROTATE_QUADSET_LEFT, RUBIX_CUBE_ROTATE_COUNTERCLOCKWISE },
			{ RUBIX_CUBE_ROTATE_QUADSET_FULL, RUBIX_CUBE_ROTATE_COUNTERCLOCKWISE }
		}
	},
	/* RIGHT */
	{
		.corner_quadset = {
			{ 0, 2 },
			{ 2, 2 },
			{ 0, 8 },
			{ 2, 8 }

		},
		.side_quadset = {
			{ 1, 2 },
			{ 2, 5 },
			{ 0, 5 },
			{ 1, 8 }
		},
		.subrotation_set = {
			{ RUBIX_CUBE_ROTATE_QUADSET_RIGHT, RUBIX_CUBE_ROTATE_UP },
			{ RUBIX_CUBE_ROTATE_QUADSET_LEFT, RUBIX_CUBE_ROTATE_DOWN },
			{ RUBIX_CUBE_ROTATE_QUADSET_FULL, RUBIX_CUBE_ROTATE_DOWN }
		}
	},
	/* RIGHT */
	{
		.corner_quadset = {
			{ 2, 0 },
			{ 0, 0 },
			{ 2, 6 },
			{ 0, 6 }

		},
		.side_quadset = {
			{ 1, 0 },
			{ 0, 3 },
			{ 2, 3 },
			{ 1, 6 }
		},
		.subrotation_set = {
			{ RUBIX_CUBE_ROTATE_QUADSET_RIGHT, RUBIX_CUBE_ROTATE_DOWN },
			{ RUBIX_CUBE_ROTATE_QUADSET_LEFT, RUBIX_CUBE_ROTATE_UP },
			{ RUBIX_CUBE_ROTATE_QUADSET_FULL, RUBIX_CUBE_ROTATE_UP }
		}
	},
	/* BACK */
	{
		.corner_quadset = {
			{ 2, 2 },
			{ 2, 0 },
			{ 2, 8 },
			{ 2, 6 }

		},
		.side_quadset = {
			{ 2, 1 },
			{ 2, 3 },
			{ 2, 5 },
			{ 2, 7 }
		},
		.subrotation_set = {
			{ RUBIX_CUBE_ROTATE_QUADSET_RIGHT, RUBIX_CUBE_ROTATE_COUNTERCLOCKWISE },
			{ RUBIX_CUBE_ROTATE_QUADSET_LEFT, RUBIX_CUBE_ROTATE_CLOCKWISE },
			{ RUBIX_CUBE_ROTATE_QUADSET_FULL, RUBIX_CUBE_ROTATE_CLOCKWISE }
		}
	},
	/* BOTTOM */
	{
		.corner_quadset = {
			{ 0, 6 },
			{ 0, 8 },
			{ 2, 6 },
			{ 2, 8 }

		},
		.side_quadset = {
			{ 0, 7 },
			{ 1, 8 },
			{ 1, 6 },
			{ 2, 7 }
		},
		.subrotation_set = {
			{ RUBIX_CUBE_ROTATE_QUADSET_RIGHT, RUBIX_CUBE_ROTATE_RIGHT },
			{ RUBIX_CUBE_ROTATE_QUADSET_LEFT, RUBIX_CUBE_ROTATE_LEFT },
			{ RUBIX_CUBE_ROTATE_QUADSET_FULL, RUBIX_CUBE_ROTATE_LEFT }
		}
	}

} ;
/* Generic face rotate */
void rubix_cube_rotate_face(RubixCube * pRubix_cube, RubixCubeSide side, RubixCubeFaceRotation face_rotation) {

	RubixCubePieceReference * corner_refs  = rubix_cube_rotation_table[side].corner_quadset ;
	RubixCubePieceReference * side_refs = rubix_cube_rotation_table[side].side_quadset ;

	RubixCubePiece * corner_piece_quadset[RUBIX_CUBE_PIECES_PER_QUADSET] = {
			&pRubix_cube->planes[corner_refs[0].plane][corner_refs[0].index],
			&pRubix_cube->planes[corner_refs[1].plane][corner_refs[1].index],
			&pRubix_cube->planes[corner_refs[2].plane][corner_refs[2].index],
			&pRubix_cube->planes[corner_refs[3].plane][corner_refs[3].index]
	} ;

	RubixCubePiece * side_piece_quadset[RUBIX_CUBE_PIECES_PER_QUADSET] = {
			&pRubix_cube->planes[side_refs[0].plane][side_refs[0].index],
			&pRubix_cube->planes[side_refs[1].plane][side_refs[1].index],
			&pRubix_cube->planes[side_refs[2].plane][side_refs[2].index],
			&pRubix_cube->planes[side_refs[3].plane][side_refs[3].index]
	} ;

	rubix_cube_rotate_piece_quadset(corner_piece_quadset,
			rubix_cube_rotation_table[side].subrotation_set[face_rotation].set_rotation,
			rubix_cube_rotation_table[side].subrotation_set[face_rotation].piece_rotation
	) ;

	rubix_cube_rotate_piece_quadset(side_piece_quadset,
			rubix_cube_rotation_table[side].subrotation_set[face_rotation].set_rotation,
			rubix_cube_rotation_table[side].subrotation_set[face_rotation].piece_rotation
	) ;
}

inline void rubix_cube_apply_move(RubixCube * pRubixCube, RubixCubeMove * move) {
	if (move) rubix_cube_rotate_face(pRubixCube,move->side,move->rotation) ;
}

inline void rubix_cube_unapply_move(RubixCube * pRubixCube, RubixCubeMove * move) {
	if(move) {
		if(move->rotation == RUBIX_CUBE_FACE_ROTATION_CLOCKWISE) {
			move->rotation = RUBIX_CUBE_FACE_ROTATION_COUNTERCLOCKWISE ;
		}
		else
		if(move->rotation == RUBIX_CUBE_FACE_ROTATION_COUNTERCLOCKWISE) {
			move->rotation = RUBIX_CUBE_FACE_ROTATION_CLOCKWISE ;
		}
		rubix_cube_apply_move(pRubixCube,move) ;
	}
}

inline RubixCubeMove rubix_cube_generate_random_move() {
	return (RubixCubeMove){
		.side 		= (RubixCubeSide)((rand() % 6)),
		.rotation 	= (RubixCubeFaceRotation)((rand() % 3) )
	} ;
}

void rubix_cube_generate_moves_from_seed(RubixCubeSeed seed, size_t number_of_moves, RubixCubeMove * dest) {
	/* validity of @dest is a preconditon, so expect SIGSEGV if you ignore that requirement */
	/* I'm not going to save any users, but I will print a message */
	assert(dest) ;

	srand(seed) ;
	for(size_t i = 0; i < number_of_moves; ++i) {
		dest[i] = rubix_cube_generate_random_move() ;
	}

}

inline const char * rubix_cube_get_face_rotation_string(RubixCubeFaceRotation face_rotation) {
	return RUBIX_CUBE_FACE_ROTATION_STRINGS[face_rotation] ;
}

inline const char * rubix_cube_get_side_string(RubixCubeSide side) {
	return RUBIX_CUBE_SIDE_STRINGS[side] ;
}

inline void rubix_cube_print_move_string(RubixCubeMove * move) {
	if(!move) {
		printf("no-op rotation of no face\n") ;
	}
	printf("%s rotation of %s face\n",
		rubix_cube_get_face_rotation_string(move->rotation),
		rubix_cube_get_side_string(move->side+1) /* not elegant, but index must be fixed */
	) ;
}

void rubix_cube_solve_scrambled_from_seed(RubixCube * pRubix_cube, RubixCubeSeed seed) {
	srand(seed) ;
	RubixCubeMove moves[RUBIX_CUBE_SCRAMBLE_INTENSITY];
	rubix_cube_generate_moves_from_seed(seed,RUBIX_CUBE_SCRAMBLE_INTENSITY,moves) ;

	for(size_t i = RUBIX_CUBE_SCRAMBLE_INTENSITY ; i > 0; --i) {
		rubix_cube_unapply_move(pRubix_cube,&moves[i-1]) ;
	}

}

void rubix_cube_print_ascii_double_stdout(RubixCube * pRubix_cube) {
	rubix_cube_print_ascii_double(stdout,pRubix_cube) ; 
}

RubixCubeSeed rubix_cube_generate_seed() {
	return time(0) ;	
}

void rubix_cube_rotate_face_strings(RubixCube * pRubix_cube, const char * side, const  char * face_rotation) {
	(void)pRubix_cube ;
	printf("side: %s\nrotation: %s\n",side,face_rotation) ;
}

RubixCubeScramble * rubix_cube_scramble_allocate(RubixCubeSeed seed,size_t intensity) {
	RubixCubeScramble * pNew ;
	if (!(pNew = (RubixCubeScramble *)malloc(sizeof(RubixCubeScramble)))) return NULL ;


	/* intensity 0 will allocate a bunch of blank space for scrambles */
	pNew->capacity = intensity ? intensity : RUBIX_CUBE_SCRAMBLE_INTENSITY ;
	if(!(pNew->moves = (RubixCubeMove *)malloc(sizeof(RubixCubeMove) * pNew->capacity))) {
		free(pNew); return NULL ;
	} pNew->size = intensity ? pNew->capacity : 0 ;

	pNew->seed = seed ? seed : rubix_cube_generate_seed() ;
	rubix_cube_generate_moves_from_seed(
			pNew->seed,
			pNew->size,
			pNew->moves) ;
	return pNew ;
}

void rubix_cube_scramble_free(RubixCubeScramble * pRubix_cube_scramble) {
	if(pRubix_cube_scramble) {
		if (pRubix_cube_scramble->moves) free(pRubix_cube_scramble->moves) ;
		free(pRubix_cube_scramble) ;
	}
}

int rubix_cube_get_default_scramble_intensity() {
	return RUBIX_CUBE_SCRAMBLE_INTENSITY ;
}

void rubix_cube_apply_scramble(RubixCube * pRubix_cube, RubixCubeScramble * pScramble) {
	for(size_t i = 0; i < pScramble->size; ++i) {
		rubix_cube_apply_move(pRubix_cube,&pScramble->moves[i]) ;
	}
}

void rubix_cube_unapply_scramble(RubixCube * pRubix_cube, RubixCubeScramble * pScramble) {
	for(size_t i = pScramble->size; i > 0; --i) {
		rubix_cube_unapply_move(pRubix_cube,&pScramble->moves[i-1]) ;
	}
}
