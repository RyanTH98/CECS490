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
 *        Authors:  Alex Gonzalez(ag) - alexcbensen@gmail.com,
 *   Benjamin Gillmore (bg) - bggillmore@gmail.com
 *
 *        Company:  California State University Long Beach
 *
 * =====================================================================================
 */


#include <stdbool.h>

typedef enum _type { NO_PIECE, PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING } _type;

typedef enum _color { NO_COLOR, BLACK, WHITE } _color;

typedef struct _piece {
	_type type;
	_color color;
	int row;
	int column;
	char direction;
	bool firstMove;
} _piece;


//Description
void get_moves(_piece(*)[8],  _piece*);

//Description
//void illuminate(piece, COLOR);

//Description
bool check_legal(_piece*);

//Description
void reset_board(_piece(*)[8]);

//Description
void move_piece(_piece(*)[8], _piece*, int, int);

//Description
void print_board(_piece(*)[8]);
