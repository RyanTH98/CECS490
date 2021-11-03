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
typedef enum _space { 	
			A1,B1,C1,D1,E1,F1,G1,H1,
			A2,B2,C2,D2,E2,F2,G2,H2, 
			A3,B3,C3,D3,E3,F3,G3,H3, 
			A4,B4,C4,D4,E4,F4,G4,H4, 
			A5,B5,C5,D5,E5,F5,G5,H5, 
			A6,B6,C6,D6,E6,F6,G6,H6, 
			A7,B7,C7,D7,E7,F7,G7,H7, 
			A8,B8,C8,D8,E8,F8,G8,H8, 
		    } _space;

typedef struct _piece {
	_space position; 
	 _type type;
	_color color;
        _space legal[26];
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


void get_move(_space[2]);

/*	Function: get_moves

	Inputs:   2D Array of Chess Board,
		      Piece to be moved
			
	Outputs:  None
*/
void get_moves(_piece(*)[8],  _piece*);
void get_moves_pawn(_piece(*)[8], _piece*);
int get_moves_bishop(_piece(*)[8], _piece*, int);
int get_moves_rook(_piece(*)[8], _piece*, int);
void get_moves_knight(_piece(*)[8], _piece*);
void get_moves_queen(_piece(*)[8], _piece*);
void get_moves_king(_piece(*)[8], _piece*);

/*	Function: update_legal

	Inputs:   2D Array of Chess Board
			
	Outputs:  None
*/
void update_legal(_piece(*)[8]);

/*  Function: move_piece

	Inputs:   2D Array of Chess Board,
			  Piece to be moved,
			  Index of destination row,
			  Index of destination column

	Outputs:  None
*/
void move_piece(_piece(*)[8], _space*);


/*  Function: print_board

	Inputs:   2D Array of Chess Board,

	Outputs:  None
*/
void print_board(_piece(*)[8]);
void print_legal_moves(_piece*);

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
