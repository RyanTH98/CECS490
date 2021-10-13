/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  10/11/2021 07:25:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Authors:  Alex Gonzalez     (ag) - alexcbensen@gmail.com,
                    Benjamin Gillmore (bg) - bggillmore@gmail.com
 *
          Company:  California State University Long Beach
 *
 * =====================================================================================
 */

#include "main.h"
#include <stdio.h>

//These defines are not necessary, they are just for coloring strings on console
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


//EDIT: Put in header file (bg)

// Piece
//typedef enum type { NONE, PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING } type;
//
//typedef enum color { NONE, BLACK, WHITE } color;
//
//typedef struct _piece {
//	type  type		= NONE;
//	color color		= NONE;
//	int   row	    = -1;
//	int   column    = -1;
//	char  direction = 'X';
//	bool  firstMove = true;
//} piece;



int main(void) {

	_piece board[8][8];

	reset_board(board);

	//printf("Test\n");
    //print_board(board);

    int x, y;

    printf("Enter move:\nrow: ");
    scanf("%d", &x);

    printf("Enter move:\ncolumn: ");
    scanf("%d", &y);

    move_piece(board, &board[0][0], x, y);

    print_board(board);


	return 0;
}

void push_back(_piece (*array)[2],_piece *new_move , int num) {
    array[num][0].row = new_move->row;
    array[num][1].column = new_move->column;
}

void get_moves(_piece (*board)[8], _piece *p) {
    int row_dif, column_dif;
    int num_legal = 0, num_illegal = 0, num = 0;


    _piece legal_moves[27][2], illegal_moves[27][2];
    _piece new_move;

	switch(p->type) {
		case NO_PIECE:
			break;
		case PAWN:
            for(int c = 0; c <= 4; c++) {
                /* If a piece is moving DOWN the board */
                if (p->direction == 'D' | p->direction == 'B') {
                    switch(c) {
                        case 0:
                            /* Check BELOW for a piece */
                            row_dif    =  1;
                            column_dif =  0;
                            break;
                        case 1:
                            /* Check BELOW, DIAGONALLY left for a piece */
                            row_dif    =  1;
                            column_dif = -1;
                            break;
                        case 2:
                            /* Check BELOW, DIAGONALLY right for a piece */
                            row_dif    =  1;
                            column_dif =  1;
                            break;
                        case 3:
                            /* Special Case */
                            /* FIRST move ONLY - Check two squares BELOW for a piece */
                            if (p->firstMove == true)
                                row_dif    = 2;
                                column_dif = 0;
                            break;
                        default:
                            break;
                    }
                }

                /* If a piece is moving UP the board */
                else if(p->direction == 'U' | p->direction == 'B') {
                    switch(c) {
                        case 0:
                            /* Check ABOVE for a piece */
                            row_dif    = -1;
                            column_dif =  0;
                            break;
                        case 1:
                            /* Check ABOVE, DIAGONALLY left for a piece */
                            row_dif    = -1;
                            column_dif = -1;
                            break;
                        case 2:
                            /* Check ABOVE, DIAGONALLY right for a piece */
                            row_dif    = -1;
                            column_dif =  1;
                            break;
                        case 3:
                            /* Special Case */
                            /* FIRST move ONLY - Check two squares BELOW for a piece */
                            if (p->firstMove == true)
                                row_dif    = -2;
                                column_dif =  0;
                            break;
                        default:
                            break;
                    }
                }

                /* Add LEGAL moves to Moves 2D Array*/
                new_move = board[p->row + row_dif][p->column + column_dif];

                if (check_legal(&new_move) == true) {
                    num = num_legal++;
                    push_back(legal_moves, &new_move, num);
                }
                else if(check_legal((&new_move)) == false) {
                    num = num_illegal++;
                    push_back(illegal_moves, &new_move, num);
                }
			}
			break;
		case BISHOP:
			/* Check DIAGONALLY for a piece, until one is detected */

			/* BELOW and to the RIGHT */
			for (int i = p->row; i < 8; i++) {
				for (int j = p->column; j < 8; j++) {
					if (check_legal(&board[i][j])) {
						// Light up square in white
					} else {
						// Light up square in red
						break;
					}
				}
			}

			/* BELOW and to the LEFT */
			for (int i = p->row; i < 8; i++) {
				for (int j = p->column; j >= 0; j--) {
					if (check_legal(&board[i][j])) {
						// Light up square in white
					} else {
						// Light up square in red
						break;
					}
				}
			}

			/* ABOVE and to the Right */
			for (int i = p->row; i >= 0; i--) {
				for (int j = p->column; j > 8; j++) {
					if (check_legal(&board[i][j])) {
						// Light up square in white
					} else {
						// Light up square in red
						break;
					}
				}
			}

			/* ABOVE and to the LEFT */
			for (int i = p->row; i >= 0; i--) {
				for (int j = p->column; j >= 0; j--) {
					if (check_legal(&board[i][j])) {
						// Light up square in white
					} else {
						// Light up square in red
						break;
					}
				}
			}
			break;
		case KNIGHT:
			break;
		case ROOK:
			/* Check HORIZONTALLY or a piece, until one is detected */

			/* LEFT, until a piece is detected */
			for (int j = p->column; j <= 0; j--) {
				if (check_legal(&board[p->row][j])) {
					// Light up square in white
				} else {
					// Light up square in red
					break;
				}
			}

			/* RIGHT, until a piece is detected */
			for (int j = p->column; j > 8; j++) {
				if (check_legal(&board[p->row][j])) {
					// Light up square in white
				} else {
					// Light up square in red
					break;
				}
			}

			/* Special Case */
			/* FIRST move ONLY - Check for King to castle */
			_piece* p1 = &board[p -> row][p->column - 3];
			_piece* p2 = &board[p -> row][p->column - 2];
			_piece* p3 = &board[p -> row][p->column - 1];

			/* Check if King is in the proper place, and squares in between Rook and King are empty */
			if (p1->type == KING && check_legal(p2) && check_legal(p3)) {
				// Light up squares in between Rook and King, in white
			}
			break;
		case QUEEN:
			/* Check DIAGONALLY for a piece, until one is detected */

			/* BELOW and to the RIGHT */
			for (int i = p->row; i < 8; i++) {
				for (int j = p->column; j < 8; j++) {
					if (check_legal(&board[i][j])) {
						// Light up square in white
					}
					else {
						// Light up square in red
						break;
					}
				}
			}

			/* BELOW and to the LEFT */
			for (int i = p->row; i < 8; i++) {
				for (int j = p->column; j >= 0; j--) {
					if (check_legal(&board[i][j])) {
						// Light up square in white
					}
					else {
						// Light up square in red
						break;
					}
				}
			}

			/* ABOVE and to the Right */
			for (int i = p->row; i >= 0; i--) {
				for (int j = p->column; j > 8; j++) {
					if (check_legal(&board[i][j])) {
						// Light up square in white
					}
					else {
						// Light up square in red
						break;
					}
				}
			}

			/* ABOVE and to the LEFT */
			for (int i = p->row; i >= 0; i--) {
				for (int j = p->column; j >= 0; j--) {
					if (check_legal(&board[i][j])) {
						// Light up square in white
					}
					else {
						// Light up square in red
						break;
					}
				}
			}


			/* Check HORIZONTALLY or a piece, until one is detected */

			/* LEFT, until a piece is detected */
			for (int j = p->column; j <= 0; j--) {
				if (check_legal(&board[p->row][j])) {
					// Light up square in white
				}
				else {
					// Light up square in red
					break;
				}
			}

			/* RIGHT, until a piece is detected */
			for (int j = p->column; j > 8; j++) {
				if (check_legal(&board[p->row][j])) {
					// Light up square in white
				}
				else {
					// Light up square in red
					break;
				}
			}
			break;
		case KING:
			break;
		default:
			break;

    num = 0;
    num_legal = 0;
    num_illegal = 0;
	}
}

/*
void illuminate(piece* p, COLOR) {
	// Code for hardware LED
}
*/

bool check_legal(_piece *p) {
	return (p->type == NO_PIECE);
}

void reset_board(_piece(*board)[8]) {

	/* Assign color of all Black pieces
	   Set default direction to moving downward */
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j].color = BLACK;
			board[i][j].direction = 'D';

			if(i == 1)
				board[i][j].type = PAWN;
		}
	}

	board[0][0].type = ROOK;
	board[0][1].type = KNIGHT;
	board[0][2].type = BISHOP;
	board[0][3].type = QUEEN;
	board[0][4].type = KING;
	board[0][5].type = BISHOP;
	board[0][6].type = KNIGHT;
	board[0][7].type = ROOK;


	/* Assign color of all White pieces
       Set default direction to moving upward */
	for (int i = 6; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				board[i][j].color = WHITE;
				board[i][j].direction = 'U';

				if(i == 6)
					board[i][j].type = PAWN;
			}
	}

	board[7][0].type = ROOK;
	board[7][1].type = KNIGHT;
	board[7][2].type = BISHOP;
	board[7][3].type = QUEEN;
	board[7][4].type = KING;
	board[7][5].type = BISHOP;
	board[7][6].type = KNIGHT;
	board[7][7].type = ROOK;

	/* Assign each piece to their corresponding space on the board */
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j].row = i;
			board[i][j].column = j;

			if(i >= 2 && i <= 5)
				board[i][j].type = NO_PIECE;
		}
	}

}

/* Input: Board (2D Array), Piece being moved (Piece), row, column */
void move_piece(_piece(*board)[8], _piece *p, int x, int y) {
	if (p->firstMove == true)
		p->firstMove == false;

	board[x][y] = *p;

	p->type = NO_PIECE;
	p->color = NO_COLOR;
}

void print_board(_piece(*board)[8]){
	int i, j;

	printf("%s---------------------------------\n", KMAG);
	for(i = 0; i<8;i++){
		for(j=0;j<8;j++){
			printf("%s|", KMAG);
			printf("%s", (board[i][j].color == BLACK)?KGRN:KBLU);
			switch(board[i][j].type){
				case NO_PIECE:
					printf("   ");
					break;
				case PAWN:
					printf(" p ");
					break;
				case BISHOP:
					printf(" b ");
					break;
				case KNIGHT:
					printf(" k ");
					break;
				case ROOK:
					printf(" r ");
					break;
				case QUEEN:
					printf(" Q ");
					break;
				case KING:
					printf(" K ");
					break;
				default:
					printf("You broke something on row %d, column %d\n", i, j);
					break;
			}
		}
		printf("%s|\n---------------------------------\n", KMAG);
	}
	//printf("%s-----------------\n", KCYN);
	printf("%s", KNRM);

}
