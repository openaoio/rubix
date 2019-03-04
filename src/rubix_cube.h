#ifndef RUBIX_CUBE_H
#define RUBIX_CUBE_H "rubix_cube.h"

/* Library dependencies */

#include <stdio.h>

/* Library constants */

/* Faces/sides of a square. */
#define RUBIX_CUBE_SIDE_COUNT 6

/* NxNxN rubix cube. Note that no other values are yet supported, but might be in the future. */
#define RUBIX_CUBE_SIDE_LENGTH 3 
#define RUBIX_CUBE_PLANE_COUNT RUBIX_CUBE_SIDE_LENGTH

/* Determine the size of each plane (in pieces) as a constant at compile time */
#define RUBIX_CUBE_PIECES_PER_PLANE RUBIX_CUBE_SIDE_LENGTH * RUBIX_CUBE_SIDE_LENGTH
#define RUBIX_CUBE_PIECES_PER_FACE RUBIX_CUBE_PIECES_PER_PLANE

/* Library types */

typedef enum rubix_cube_side  {
	RUBIX_CUBE_SIDE_NULL,
	RUBIX_CUBE_SIDE_A,
	RUBIX_CUBE_SIDE_B,
	RUBIX_CUBE_SIDE_C,
	RUBIX_CUBE_SIDE_D,
	RUBIX_CUBE_SIDE_E,
	RUBIX_CUBE_SIDE_F
} RubixCubeSide  ; // TODO: change these perhaps


/* square indices */
#define RUBIX_CUBE_SQUARE_TOP RUBIX_CUBE_SIDE_A - 1
#define RUBIX_CUBE_SQUARE_FRONT RUBIX_CUBE_SIDE_B - 1
#define RUBIX_CUBE_SQUARE_RIGHT RUBIX_CUBE_SIDE_C - 1
#define RUBIX_CUBE_SQUARE_LEFT RUBIX_CUBE_SIDE_D - 1
#define RUBIX_CUBE_SQUARE_BACK RUBIX_CUBE_SIDE_E - 1
#define RUBIX_CUBE_SQUARE_BOTTOM RUBIX_CUBE_SIDE_F - 1

typedef enum rubix_cube_color  {
	RUBIX_CUBE_COLOR_NULL,
	RUBIX_CUBE_COLOR_WHITE,
	RUBIX_CUBE_COLOR_RED,
	RUBIX_CUBE_COLOR_BLUE,
	RUBIX_CUBE_COLOR_GREEN,
	RUBIX_CUBE_COLOR_ORANGE,
	RUBIX_CUBE_COLOR_YELLOW
} RubixCubeColor ;

/* Seed type of value for generation of scrambled rubix cube */
typedef unsigned long long int rubix_cube_seed_t ;

/*
 * Rubix cube model:
 *(behind) +--+--+--+                            For each slice of 9 cubes (divided by the plane of the screen),
 *  D    /  /  /  /|					there exists an array of RubixCubePiece objects
 *  ->  +--+--+--+ |					arranged (i.e. indexed) like such. Let it be called a "plane"
 *     /  /A /  /| |					+--+--+--+
 *    +--+--+--+ | |				       / 0/ 1/ 2/|
 *   /  /  /  /| |/| <-- E (behind)		      +--+--+--+ |
 *  +--+--+--+ |/| |				0-->  |0 |1 |2 |2/
 *  |  |  |  |/|C|/|      			      +--+--+--+ |
 *  +--+--+--+ |/| |       2 			3-->  |3 |4 |5 |5/
 *  |  |B |  |/| |/           _.		      +--+--+--+ |
 *  +--+--+--+ |/        1    /|		6-->  |6 |7 |8 |8/
 *  |  |  |  | /             /  		      +--+--+--+ 
 *  +--+--+--+^         0   / Direction of array of rubix cube face arrays
 *       F ^
 * (behind)|
 */



typedef struct rubix_cube_piece {
	/* Each piece is represented by an array for rubix cube colors in side order as documented above */
	RubixCubeColor sides[RUBIX_CUBE_SIDE_COUNT] ;
} RubixCubePiece ;

typedef struct rubix_cube {
	RubixCubePiece planes[RUBIX_CUBE_PLANE_COUNT][RUBIX_CUBE_PIECES_PER_PLANE] ;
} RubixCube ;

/* 2D face represented by matrix of colors (what you see when you look at a side) */
typedef struct rubix_cube_face {
	RubixCubeColor squares[RUBIX_CUBE_PIECES_PER_FACE] ;
} RubixCubeFace ;

#define RUBIX_CUBE_GET_TOP_FACE(rubix_cube_id) \
	(RubixCubeFace){ .squares = { \
			rubix_cube_id.planes[0][0].sides[RUBIX_CUBE_SQUARE_TOP], \
			rubix_cube_id.planes[0][1].sides[RUBIX_CUBE_SQUARE_TOP], \
			rubix_cube_id.planes[0][2].sides[RUBIX_CUBE_SQUARE_TOP], \
			\
			rubix_cube_id.planes[1][0].sides[RUBIX_CUBE_SQUARE_TOP], \
			rubix_cube_id.planes[1][1].sides[RUBIX_CUBE_SQUARE_TOP], \
			rubix_cube_id.planes[1][2].sides[RUBIX_CUBE_SQUARE_TOP], \
			\
			rubix_cube_id.planes[2][0].sides[RUBIX_CUBE_SQUARE_TOP], \
			rubix_cube_id.planes[2][1].sides[RUBIX_CUBE_SQUARE_TOP], \
			rubix_cube_id.planes[2][2].sides[RUBIX_CUBE_SQUARE_TOP]  \
		} \
	}

#define RUBIX_CUBE_GET_FRONT_FACE(rubix_cube_id) \
	(RubixCubeFace){ .squares = { \
			rubix_cube_id.planes[0][0].sides[RUBIX_CUBE_SQUARE_FRONT], \
			rubix_cube_id.planes[0][1].sides[RUBIX_CUBE_SQUARE_FRONT], \
			rubix_cube_id.planes[0][2].sides[RUBIX_CUBE_SQUARE_FRONT], \
			\
			rubix_cube_id.planes[0][4].sides[RUBIX_CUBE_SQUARE_FRONT], \
			rubix_cube_id.planes[0][5].sides[RUBIX_CUBE_SQUARE_FRONT], \
			rubix_cube_id.planes[0][6].sides[RUBIX_CUBE_SQUARE_FRONT], \
			\
			rubix_cube_id.planes[0][7].sides[RUBIX_CUBE_SQUARE_FRONT], \
			rubix_cube_id.planes[0][8].sides[RUBIX_CUBE_SQUARE_FRONT], \
			rubix_cube_id.planes[0][9].sides[RUBIX_CUBE_SQUARE_FRONT]  \
		} \
	}

#define RUBIX_CUBE_GET_RIGHT_FACE(rubix_cube_id) \
	(RubixCubeFace){ .squares = { \
			rubix_cube_id.planes[0][2].sides[RUBIX_CUBE_SQUARE_RIGHT], \
			rubix_cube_id.planes[0][5].sides[RUBIX_CUBE_SQUARE_RIGHT], \
			rubix_cube_id.planes[0][8].sides[RUBIX_CUBE_SQUARE_RIGHT], \
			\
			rubix_cube_id.planes[1][2].sides[RUBIX_CUBE_SQUARE_RIGHT], \
			rubix_cube_id.planes[1][5].sides[RUBIX_CUBE_SQUARE_RIGHT], \
			rubix_cube_id.planes[1][8].sides[RUBIX_CUBE_SQUARE_RIGHT], \
			\
			rubix_cube_id.planes[2][2].sides[RUBIX_CUBE_SQUARE_RIGHT], \
			rubix_cube_id.planes[2][5].sides[RUBIX_CUBE_SQUARE_RIGHT], \
			rubix_cube_id.planes[2][8].sides[RUBIX_CUBE_SQUARE_RIGHT]  \
		} \
	}

#define RUBIX_CUBE_GET_LEFT_FACE(rubix_cube_id) \
	(RubixCubeFace){ .squares = { \
			rubix_cube_id.planes[2][0].sides[RUBIX_CUBE_SQUARE_LEFT], \
			rubix_cube_id.planes[2][3].sides[RUBIX_CUBE_SQUARE_LEFT], \
			rubix_cube_id.planes[2][6].sides[RUBIX_CUBE_SQUARE_LEFT], \
			\
			rubix_cube_id.planes[1][0].sides[RUBIX_CUBE_SQUARE_LEFT], \
			rubix_cube_id.planes[1][3].sides[RUBIX_CUBE_SQUARE_LEFT], \
			rubix_cube_id.planes[1][6].sides[RUBIX_CUBE_SQUARE_LEFT], \
			\
			rubix_cube_id.planes[0][0].sides[RUBIX_CUBE_SQUARE_LEFT], \
			rubix_cube_id.planes[0][3].sides[RUBIX_CUBE_SQUARE_LEFT], \
			rubix_cube_id.planes[0][6].sides[RUBIX_CUBE_SQUARE_LEFT]  \
		} \
	}

#define RUBIX_CUBE_GET_BACK_FACE(rubix_cube_id) \
	(RubixCubeFace){ .squares = { \
			rubix_cube_id.planes[2][2].sides[RUBIX_CUBE_SQUARE_BACK], \
			rubix_cube_id.planes[2][1].sides[RUBIX_CUBE_SQUARE_BACK], \
			rubix_cube_id.planes[2][0].sides[RUBIX_CUBE_SQUARE_BACK], \
			\
			rubix_cube_id.planes[2][5].sides[RUBIX_CUBE_SQUARE_BACK], \
			rubix_cube_id.planes[2][4].sides[RUBIX_CUBE_SQUARE_BACK], \
			rubix_cube_id.planes[2][3].sides[RUBIX_CUBE_SQUARE_BACK], \
			\
			rubix_cube_id.planes[2][8].sides[RUBIX_CUBE_SQUARE_BACK], \
			rubix_cube_id.planes[2][7].sides[RUBIX_CUBE_SQUARE_BACK], \
			rubix_cube_id.planes[2][6].sides[RUBIX_CUBE_SQUARE_BACK]  \
		} \
	}

#define RUBIX_CUBE_GET_BOTTOM_FACE(rubix_cube_id) \
	(RubixCubeFace){ .squares = { \
			rubix_cube_id.planes[0][6].sides[RUBIX_CUBE_SQUARE_BOTTOM], \
			rubix_cube_id.planes[0][7].sides[RUBIX_CUBE_SQUARE_BOTTOM], \
			rubix_cube_id.planes[0][8].sides[RUBIX_CUBE_SQUARE_BOTTOM], \
			\
			rubix_cube_id.planes[1][6].sides[RUBIX_CUBE_SQUARE_BOTTOM], \
			rubix_cube_id.planes[1][7].sides[RUBIX_CUBE_SQUARE_BOTTOM], \
			rubix_cube_id.planes[1][8].sides[RUBIX_CUBE_SQUARE_BOTTOM], \
			\
			rubix_cube_id.planes[2][6].sides[RUBIX_CUBE_SQUARE_BOTTOM], \
			rubix_cube_id.planes[2][7].sides[RUBIX_CUBE_SQUARE_BOTTOM], \
			rubix_cube_id.planes[2][8].sides[RUBIX_CUBE_SQUARE_BOTTOM]  \
		} \
	}

#define RUBIX_CUBE_GET_INVALID_FACE(rubix_cube_id) \
	(RubixCubeFace){ .squares = { \
			RUBIX_CUBE_COLOR_NULL, \
			RUBIX_CUBE_COLOR_NULL, \
			RUBIX_CUBE_COLOR_NULL, \
			\
			RUBIX_CUBE_COLOR_NULL, \
			RUBIX_CUBE_COLOR_NULL, \
			RUBIX_CUBE_COLOR_NULL, \
			\
			RUBIX_CUBE_COLOR_NULL, \
			RUBIX_CUBE_COLOR_NULL, \
			RUBIX_CUBE_COLOR_NULL  \
		} \
	}

#define RUBIX_CUBE_PIECE_NULL (RubixCubePiece){ .sides = {  RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL } }

/* This is the identity */
#define RUBIX_CUBE_SOLVED_LITERAL \
	(RubixCube) { \
		.planes = { \
				{ \
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_WHITE, RUBIX_CUBE_COLOR_RED, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_GREEN, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL } },\
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_WHITE, RUBIX_CUBE_COLOR_RED, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL } }, \
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_WHITE, RUBIX_CUBE_COLOR_RED, RUBIX_CUBE_COLOR_BLUE, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL } },\
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_RED, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_GREEN, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL } },\
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_RED, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL } },\
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_RED, RUBIX_CUBE_COLOR_BLUE, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL } },\
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_RED, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_GREEN, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_YELLOW } },\
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_RED, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_YELLOW } },\
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_RED, RUBIX_CUBE_COLOR_BLUE, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_YELLOW } }\
				},\
				{ \
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_WHITE, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_GREEN, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL } },\
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_WHITE, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL } }, \
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_WHITE, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_BLUE, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL } },\
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_GREEN, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL } },\
					RUBIX_CUBE_PIECE_NULL, /* Center of the cube */ \
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_BLUE, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL } },\
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_GREEN, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_YELLOW } },\
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_YELLOW } },\
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_BLUE, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_YELLOW } }\
				},\
				{ \
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_WHITE, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_GREEN, RUBIX_CUBE_COLOR_ORANGE, RUBIX_CUBE_COLOR_NULL } },\
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_WHITE, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_ORANGE, RUBIX_CUBE_COLOR_NULL } }, \
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_WHITE, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_BLUE, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_ORANGE, RUBIX_CUBE_COLOR_NULL } },\
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_GREEN, RUBIX_CUBE_COLOR_ORANGE, RUBIX_CUBE_COLOR_NULL } },\
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_ORANGE, RUBIX_CUBE_COLOR_NULL } },\
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_BLUE, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_ORANGE, RUBIX_CUBE_COLOR_NULL } },\
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_GREEN, RUBIX_CUBE_COLOR_ORANGE, RUBIX_CUBE_COLOR_YELLOW } },\
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_ORANGE, RUBIX_CUBE_COLOR_YELLOW } },\
					(RubixCubePiece){ .sides = { RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_BLUE, RUBIX_CUBE_COLOR_NULL, RUBIX_CUBE_COLOR_ORANGE, RUBIX_CUBE_COLOR_YELLOW } }\
				}\
			}\
		} \

/* Library Functions */

RubixCube rubix_cube_generate_solved(void) ;

RubixCube rubix_cube_generate_scrambled(rubix_cube_seed_t seed) ;

void rubix_cube_print_ascii(FILE * output_file, RubixCube * pRubix_cube) ;

void rubix_cube_print_face_ascii(FILE * output_file, RubixCube * pRubix_cube, RubixCubeSide side) ;

#endif // RUBIX_CUBE_H
