#ifndef RUBIX_CUBE_H
#define RUBIX_CUBE_H "rubix_cube.h"

/* Library dependencies */

#include <stdio.h>
#include <pthread.h> /* for monkeys */

/* Library constants */

/* Faces/sides of a square. */
#define RUBIX_CUBE_SIDE_COUNT 		6

/* NxNxN rubix cube. Note that no other values are yet supported, but might be in the future. */
#define RUBIX_CUBE_SIDE_LENGTH 		3 
#define RUBIX_CUBE_PLANE_COUNT 		RUBIX_CUBE_SIDE_LENGTH

/* Determine the size of each plane (in pieces) as a constant at compile time */
#define RUBIX_CUBE_PIECES_PER_PLANE 	RUBIX_CUBE_SIDE_LENGTH * RUBIX_CUBE_SIDE_LENGTH
#define RUBIX_CUBE_PIECES_PER_FACE 	RUBIX_CUBE_PIECES_PER_PLANE

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
#define RUBIX_CUBE_SQUARE_TOP 		RUBIX_CUBE_SIDE_A - 1
#define RUBIX_CUBE_SQUARE_FRONT 	RUBIX_CUBE_SIDE_B - 1
#define RUBIX_CUBE_SQUARE_RIGHT 	RUBIX_CUBE_SIDE_C - 1
#define RUBIX_CUBE_SQUARE_LEFT 		RUBIX_CUBE_SIDE_D - 1
#define RUBIX_CUBE_SQUARE_BACK 		RUBIX_CUBE_SIDE_E - 1
#define RUBIX_CUBE_SQUARE_BOTTOM 	RUBIX_CUBE_SIDE_F - 1

/* alias squares as faces for readability */
#define RUBIX_CUBE_FACE_TOP 		RUBIX_CUBE_SQUARE_TOP 
#define RUBIX_CUBE_FACE_FRONT 		RUBIX_CUBE_SQUARE_FRONT 
#define RUBIX_CUBE_FACE_RIGHT 		RUBIX_CUBE_SQUARE_RIGHT
#define RUBIX_CUBE_FACE_LEFT 		RUBIX_CUBE_SQUARE_LEFT
#define RUBIX_CUBE_FACE_BACK 		RUBIX_CUBE_SQUARE_BACK
#define RUBIX_CUBE_FACE_BOTTOM 		RUBIX_CUBE_SQUARE_BOTTOM

typedef enum rubix_cube_color  {
	RUBIX_CUBE_COLOR_NULL,
	RUBIX_CUBE_COLOR_WHITE,
	RUBIX_CUBE_COLOR_RED,
	RUBIX_CUBE_COLOR_BLUE,
	RUBIX_CUBE_COLOR_GREEN,
	RUBIX_CUBE_COLOR_ORANGE,
	RUBIX_CUBE_COLOR_YELLOW
} RubixCubeColor ;

typedef enum rubix_cube_rotation {
	RUBIX_CUBE_ROTATE_UP,
	RUBIX_CUBE_ROTATE_DOWN,
	RUBIX_CUBE_ROTATE_RIGHT,
	RUBIX_CUBE_ROTATE_LEFT,
	RUBIX_CUBE_ROTATE_CLOCKWISE,
	RUBIX_CUBE_ROTATE_COUNTERCLOCKWISE,
} RubixCubeRotation ;

typedef enum rubix_cube_rotation_quadset {
	RUBIX_CUBE_ROTATE_QUADSET_RIGHT,	/*  90 degree (aka  pi/2 radian) rotation */
	RUBIX_CUBE_ROTATE_QUADSET_LEFT,     	/* -90 degree (aka -pi/2 radian) rotation */
	RUBIX_CUBE_ROTATE_QUADSET_FULL  	/* 180 degree (aka  pi   radian) rotation */
} RubixCubeRotationQuadset ;
 /* Important for dealing with quadsets */
#define RUBIX_CUBE_PIECES_PER_QUADSET 	4

typedef enum rubix_cube_face_rotation {
	RUBIX_CUBE_FACE_ROTATION_CLOCKWISE,
	RUBIX_CUBE_FACE_ROTATION_COUNTERCLOCKWISE,
	RUBIX_CUBE_FACE_ROTATION_DOUBLE
} RubixCubeFaceRotation ;
#define RUBIX_CUBE_FACE_ROTATION_COUNT 	3

/* Seed type of value for generation of scrambled rubix cube */
typedef unsigned long long int RubixCubeSeed ;
// TODO: scrambling algorithm

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
 *
 * Library functions will maintain this "perspective" unless otherwise noted.
 * rubix_cube_rotate_quadset() is a notable exception
 */



typedef struct rubix_cube_piece {
	/* Each piece is represented by an array for rubix cube colors in side order as documented above */
	RubixCubeColor sides[RUBIX_CUBE_SIDE_COUNT] ;
} RubixCubePiece ;

size_t rubix_cube_global_count ;
typedef struct rubix_cube {
	size_t ID ;
	RubixCubePiece planes[RUBIX_CUBE_PLANE_COUNT][RUBIX_CUBE_PIECES_PER_PLANE] ;
} RubixCube ;

/* 2D face represented by matrix of colors (what you see when you look at a side) */
typedef struct rubix_cube_face {
	RubixCubeColor squares[RUBIX_CUBE_PIECES_PER_FACE] ;
} RubixCubeFace ;

#define RUBIX_CUBE_GET_TOP_FACE(rubix_cube_id) \
	(RubixCubeFace){ .squares = { \
			rubix_cube_id.planes[2][0].sides[RUBIX_CUBE_SQUARE_TOP], \
			rubix_cube_id.planes[2][1].sides[RUBIX_CUBE_SQUARE_TOP], \
			rubix_cube_id.planes[2][2].sides[RUBIX_CUBE_SQUARE_TOP], \
			\
			rubix_cube_id.planes[1][0].sides[RUBIX_CUBE_SQUARE_TOP], \
			rubix_cube_id.planes[1][1].sides[RUBIX_CUBE_SQUARE_TOP], \
			rubix_cube_id.planes[1][2].sides[RUBIX_CUBE_SQUARE_TOP], \
			\
			rubix_cube_id.planes[0][0].sides[RUBIX_CUBE_SQUARE_TOP], \
			rubix_cube_id.planes[0][1].sides[RUBIX_CUBE_SQUARE_TOP], \
			rubix_cube_id.planes[0][2].sides[RUBIX_CUBE_SQUARE_TOP]  \
			\
		} \
	}

#define RUBIX_CUBE_GET_FRONT_FACE(rubix_cube_id) \
	(RubixCubeFace){ .squares = { \
			rubix_cube_id.planes[0][0].sides[RUBIX_CUBE_SQUARE_FRONT], \
			rubix_cube_id.planes[0][1].sides[RUBIX_CUBE_SQUARE_FRONT], \
			rubix_cube_id.planes[0][2].sides[RUBIX_CUBE_SQUARE_FRONT], \
			\
			rubix_cube_id.planes[0][3].sides[RUBIX_CUBE_SQUARE_FRONT], \
			rubix_cube_id.planes[0][4].sides[RUBIX_CUBE_SQUARE_FRONT], \
			rubix_cube_id.planes[0][5].sides[RUBIX_CUBE_SQUARE_FRONT], \
			\
			rubix_cube_id.planes[0][6].sides[RUBIX_CUBE_SQUARE_FRONT], \
			rubix_cube_id.planes[0][7].sides[RUBIX_CUBE_SQUARE_FRONT], \
			rubix_cube_id.planes[0][8].sides[RUBIX_CUBE_SQUARE_FRONT]  \
		} \
	}

#define RUBIX_CUBE_GET_RIGHT_FACE(rubix_cube_id) \
	(RubixCubeFace){ .squares = { \
			rubix_cube_id.planes[0][2].sides[RUBIX_CUBE_SQUARE_RIGHT], \
			rubix_cube_id.planes[1][2].sides[RUBIX_CUBE_SQUARE_RIGHT], \
			rubix_cube_id.planes[2][2].sides[RUBIX_CUBE_SQUARE_RIGHT], \
			\
			rubix_cube_id.planes[0][5].sides[RUBIX_CUBE_SQUARE_RIGHT], \
			rubix_cube_id.planes[1][5].sides[RUBIX_CUBE_SQUARE_RIGHT], \
			rubix_cube_id.planes[2][5].sides[RUBIX_CUBE_SQUARE_RIGHT], \
			\
			rubix_cube_id.planes[0][8].sides[RUBIX_CUBE_SQUARE_RIGHT], \
			rubix_cube_id.planes[1][8].sides[RUBIX_CUBE_SQUARE_RIGHT], \
			rubix_cube_id.planes[2][8].sides[RUBIX_CUBE_SQUARE_RIGHT]  \
		} \
	}

#define RUBIX_CUBE_GET_LEFT_FACE(rubix_cube_id) \
	(RubixCubeFace){ .squares = { \
			rubix_cube_id.planes[2][0].sides[RUBIX_CUBE_SQUARE_LEFT], \
			rubix_cube_id.planes[1][0].sides[RUBIX_CUBE_SQUARE_LEFT], \
			rubix_cube_id.planes[0][0].sides[RUBIX_CUBE_SQUARE_LEFT], \
			\
			rubix_cube_id.planes[2][3].sides[RUBIX_CUBE_SQUARE_LEFT], \
			rubix_cube_id.planes[1][3].sides[RUBIX_CUBE_SQUARE_LEFT], \
			rubix_cube_id.planes[0][3].sides[RUBIX_CUBE_SQUARE_LEFT], \
			\
			rubix_cube_id.planes[2][6].sides[RUBIX_CUBE_SQUARE_LEFT], \
			rubix_cube_id.planes[1][6].sides[RUBIX_CUBE_SQUARE_LEFT], \
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

#define RUBIX_CUBE_GET_INVALID_FACE(rubix_cube_id) RUBIX_CUBE_FACE_NULL

#define RUBIX_CUBE_FACE_NULL \
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

#define RUBIX_CUBE_IDENTITY RUBIX_CUBE_SOLVED_LITERAL

typedef struct rubix_cube_subrotation_set {
	RubixCubeRotationQuadset 	set_rotation ;
	RubixCubeRotation 		piece_rotation ;	
} RubixCubeSubrotationSet ;

typedef struct rubix_cube_piece_reference {
	size_t 				plane ;
	size_t 				index ;
} RubixCubePieceReference ;

typedef struct rubix_cube_face_rotation_data {
	RubixCubePieceReference		corner_quadset	[RUBIX_CUBE_PIECES_PER_QUADSET] ;
	RubixCubePieceReference		side_quadset	[RUBIX_CUBE_PIECES_PER_QUADSET] ;
	RubixCubeSubrotationSet 	subrotation_set	[RUBIX_CUBE_FACE_ROTATION_COUNT] ;
} RubixCubeFaceRotationData ;

typedef struct rubix_cube_move {
	RubixCubeSide		 	side ;
	RubixCubeFaceRotation 		rotation ;
} RubixCubeMove ;

typedef struct rubix_cube_scramble {
	RubixCubeMove * 		moves ;
	size_t				size ;
	size_t 				capacity ;
	RubixCubeSeed 			seed ;
} RubixCubeScramble ;

/* Library Functions */

RubixCube rubix_cube_generate_solved() ;
RubixCube * rubix_cube_allocate_solved() ;

#define RUBIX_CUBE_SCRAMBLE_INTENSITY 50
RubixCube rubix_cube_generate_scrambled(RubixCubeSeed seed) ;
RubixCube * rubix_cube_allocate_scrambled(RubixCubeSeed seed) ;

void rubix_cube_print_ascii(FILE * output_file, RubixCube * pRubix_cube) ;
void rubix_cube_print_ascii_double(FILE * output_file, RubixCube * pRubix_cube) ;
void rubix_cube_print_ascii_stdout(RubixCube * pRubix_cube) ;
void rubix_cube_print_ascii_double_stdout(RubixCube * pRubix_cube) ;

void rubix_cube_print_face_ascii(FILE * output_file, RubixCube * pRubix_cube, RubixCubeSide side) ;

void rubix_cube_free(RubixCube * pRubix_cube) ;
void rubix_cube_rotate_front_face(RubixCube * pRubix_cube, RubixCubeFaceRotation face_rotation) ;
void rubix_cube_rotate_top_face(RubixCube * pRubix_cube, RubixCubeFaceRotation face_rotation) ;

void rubix_cube_swap_pieces(RubixCubePiece * first, RubixCubePiece * second) ;

void rubix_cube_print_piece(RubixCubePiece piece) ;
void rubix_cube_print_piece_from_cube(RubixCube * pRubix_cube, unsigned plane, unsigned index) ;

int rubix_cube_is_solved(RubixCube * pRubix_cube) ;
int rubix_cube_equivelence_check(RubixCube * first, RubixCube * second) ;
int rubix_cube_get_default_scramble_intensity() ;

/*
 * TODO: documentation
 */
void rubix_cube_rotate_face(RubixCube * pRubix_cube, RubixCubeSide side, RubixCubeFaceRotation face_rotation) ;
void rubix_cube_rotate_face_strings(RubixCube * pRubix_cube, const char * side, const  char * face_rotation) ;

void rubix_cube_apply_move(RubixCube * pRubixCube, RubixCubeMove * move) ;
void rubix_cube_unapply_move(RubixCube * pRubixCube, RubixCubeMove * move) ;
void rubix_cube_print_move_string(RubixCubeMove * move) ;

RubixCubeMove rubix_cube_generate_random_move() ;
void rubix_cube_generate_moves_from_seed(RubixCubeSeed seed, size_t number_of_moves, RubixCubeMove * dest) ;

const char * rubix_cube_get_side_string(RubixCubeSide side) ;
const char * rubix_cube_get_face_rotation_string(RubixCubeFaceRotation rotation) ;

void rubix_cube_solve_scrambled_from_seed(RubixCube * pRubix_cube, RubixCubeSeed seed) ;

/* begin scramble section */
RubixCubeScramble * rubix_cube_scramble_allocate(RubixCubeSeed seed,size_t intensity) ;
void rubix_cube_scramble_free() ;

void rubix_cube_apply_scramble(RubixCube * pRubix_cube, RubixCubeScramble * pScramble) ;
void rubix_cube_unapply_scramble(RubixCube * pRubix_cube, RubixCubeScramble * pScramble) ;

RubixCubeSeed rubix_cube_generate_seed() ;

/* begin monkey section */

typedef enum rubix_cube_monkey_office {
	RUBIX_CUBE_MONKEY_ENTREPRENEUR, /* this one works 80+ hours per week */
	RUBIX_CUBE_MONKEY_MANAGER,	/* this one works close to 60 */
	RUBIX_CUBE_MONKEY_WORKER	/* a purely 9-5 kind of monkey */
} RubixCubeMonkeyOffice ;

typedef struct rubix_cube_monkey RubixCubeMonkey ;
typedef struct rubix_cube_monkey {
	RubixCube * 			cube ;
	pthread_mutex_t			cube_mutex ;
	RubixCubeMonkeyOffice 		office ;
	RubixCubeScramble * 		history ;
	RubixCubeMonkey * 		direct_reports ;
	size_t 				direct_report_count ;
} RubixCubeMonkey ;

#endif // RUBIX_CUBE_H
