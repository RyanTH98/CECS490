/*
 * =====================================================================================
 *
 *       Filename:  main.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  10/12/2021 02:43:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Authors:  Alex Gonzalez(ag)	   - alexcbensen@gmail.com,
 *				    Benjamin Gillmore (bg) - bggillmore@gmail.com
 *
 *        Company:  California State University Long Beach
 *
 * =====================================================================================
 */


#include <stdbool.h>

typedef enum _type { NO_PIECE, PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING } _type;

typedef enum _color { NO_COLOR, BLACK, WHITE } _color;

typedef struct _piece {
	  bool firstMove; 
	   int row, column; 
	  char row_char, direction;
	 _type type;
	_color color;
	
} _piece;


/*  Function: check_legal

	Inputs:   Space on board to check,

	Outputs:  Boolean value - Whether the move is legal or not
*/
bool check_for(_type, _piece*);


char get_char();

/*	Function: get_int

	Inputs:   Lower and upper bounds for integer

	Outputs:  None
*/
int get_int(int lower, int upper);


void get_move(int(*)[4]);

/*	Function: get_moves

	Inputs:   2D Array of Chess Board,
		      Piece to be moved
			
	Outputs:  None
*/
void get_moves(_piece(*)[8],  _piece*);


/*  Function: move_piece

	Inputs:   2D Array of Chess Board,
			  Piece to be moved,
			  Index of destination row,
			  Index of destination column

	Outputs:  None
*/
void move_piece(_piece(*)[8], _piece*, int x, int y);


/*  Function: print_board

	Inputs:   2D Array of Chess Board,

	Outputs:  None
*/
void print_board(_piece(*)[8]);


/*	Function: push_back
	
	Inputs:   Array to add Piece to,
		      Piece to be added to the array,
		      Index of array piece should be inserted into

	Outputs:  None
*/
void push_back(_piece(*)[2], _piece* new_move, int num);


/*  Function: reset_legal

	Inputs:   2D Array of Chess Board,

	Outputs:  None
*/
void reset_board(_piece(*)[8]);


int to_int(char c);