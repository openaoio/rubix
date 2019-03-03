#ifndef RUBIX_CUBE_H
#define RUBIX_CUBE_H "rubix_cube.h"

#define RUBIX_CUBE_SIDE_COUNT 6

typedef enum rubix_cube_side  {
	RUBIX_CUBE_SIDE_NULL,
	RUBIX_CUBE_SIDE_A,
	RUBIX_CUBE_SIDE_B,
	RUBIX_CUBE_SUDE_C,
	RUBIX_CUBE_SIDE_D,
	RUBIX_CUBE_SIDE_E,
	RUBIX_CUBE_SIDE_F
} RubixCubeSide  ; // TODO: change these perhaps

typedef enum rubix_cube_color  {
	RUBIX_CUBE_COLOR_NULL,
	RUBIX_CUBE_COLOR_WHITE,
	RUBIX_CUBE_COLOR_RED,
	RUBIX_CUBE_COLOR_BLUE,
	RUBIX_CUBE_COLOR_GREEN,
	RUBIX_CUBE_COLOR_ORANGE,
	RUBIX_CUBE_COLOR_YELLOW
} RubixCubeColor ;

/*
 * Rubix cube model:
 *        +--+--+--+                            For each slice of 9 cubes (divided by the plane of the screen),
 *  D    /  /  /  /|					there exists an array of RubixCubePiece objects
 *  ->  +--+--+--+ |					arranged (i.e. indexed) like such:
 *     /  /A /  /| |					+--+--+--+
 *    +--+--+--+ |/|				       / 0/ 1/ 2/|
 *   /  /  /  /| | | <-- E 			      +--+--+--+ |
 *  +--+--+--+ |/| |				0-->  |0 |1 |2 |2/
 *  |  |  |  |/|C|/|      			      +--+--+--+ |
 *  +--+--+--+ |/| |       2 			3-->  |3 |4 |5 |5/
 *  |  |B |  |/| |/           _.		      +--+--+--+ |
 *  +--+--+--+ |/        1    /|		6-->  |6 |7 |8 |8/
 *  |  |  |  | /             /  		      +--+--+--+ 
 *  +--+--+--+^         0   / Direction of array of rubix cube face arrays
 *    F ^
 *      |
 */



typedef struct rubix_cube_piece {
	/* Each piece is represented by an array for rubix cube colors in side order as documented above */
	RubixCubeColor sides[RUBIX_CUBE_SIDE_COUNT] ;
} RubixCubePiece ;

typedef struct rubix_cube {
		
} RubixCube ;

#endif // RUBIX_CUBE_H
