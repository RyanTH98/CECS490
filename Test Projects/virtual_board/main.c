// Author:  Alex Gonzalez
// Project: Smart Chess Board

// Piece
typedef enum type { NONE, PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING } type;

typedef enum color { NONE, BLACK, WHITE } color;

typedef struct _piece {
	type  type		= NONE;
	color color		= NONE;
	int   row	    = -1;
	int   column    = -1;
	char  direction = 'X';
	bool  firstMove = true;
} piece;



int main(void) {

	piece board[7][7];

	reset_board(&board);

	printf("Test");

	return 0;
}


void get_moves(piece *board, piece *p) {
	switch(p) {
		case NONE:
			break;
		case PAWN:
			/* If a piece is moving DOWN the board */
			if (p->direction == 'D') {

				/* Check BELOW for a piece */
				if (check_legal(board[p->row - 1][p->column]))
					// Light up square in white

				/* Check BELOW, DIAGONALLY for a piece */
				if (!check_legal(board[p->row - 1][p->column - 1]))
					// Light up square in red
				if (!check_legal(board[p->row - 1][p->column + 1]))
					// Light up square in red


				/* Special Case */
				/* FIRST move ONLY - Check two squares BELOW for a piece */
				if (p->firstMove == true)
					if (check_legal(board[p->row - 2][p->column]))
						// Light up square in white/
		    }

			/* If a piece is moving UP the board */
			else if (p->direction == 'D') {

				/* Check ABOVE for a piece */
				if (check_legal(board[p->row + 1][p->column]))
					// Light up square in white

				/* Check ABOVE, DIAGONALLY for a piece */
				if (!check_legal(board[p->row + 1][p->column - 1]))
					// Light up square in red
				if (!check_legal(board[p->row + 1][p->column + 1]))
					// Light up square in red

							
				/* Special Case */
				/* FIRST move ONLY - Check two squares BELOW for a piece */
				if (p->firstMove == true)
					if (check_legal(board[p->row - 2][p->column]))
						// Light up square in white/
			}
			break;
		case BISHOP:
			/* Check DIAGONALLY for a piece, until one is detected */

			/* BELOW and to the RIGHT */
			for (int i = p->row; i < 8; i++) {
				for (int j = p->column; j < 8; j++) {
					if (check_legal(board[i][j])) {
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
					if (check_legal(board[i][j])) {
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
					if (check_legal(board[i][j])) {
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
					if (check_legal(board[i][j])) {
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
				if (check_legal(board[p->row][j])) {
					// Light up square in white
				} else {
					// Light up square in red
					break;
				}
			}

			/* RIGHT, until a piece is detected */
			for (int j = p->column; j > 8; j++) {
				if (check_legal(board[p->row][j])) {
					// Light up square in white
				} else {
					// Light up square in red
					break;
				}
			}

			/* Special Case */
			/* FIRST move ONLY - Check for King to castle */
			piece p1 = board[p - row][p->column - 3];
			piece p2 = board[p - row][p->column - 2];
			piece p3 = board[p - row][p->column - 1];
			
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
					if (check_legal(board[i][j])) {
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
					if (check_legal(board[i][j])) {
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
					if (check_legal(board[i][j])) {
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
					if (check_legal(board[i][j])) {
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
				if (check_legal(board[p->row][j])) {
					// Light up square in white
				}
				else {
					// Light up square in red
					break;
				}
			}

			/* RIGHT, until a piece is detected */
			for (int j = p->column; j > 8; j++) {
				if (check_legal(board[p->row][j])) {
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
	}
}



/*
void illuminate(piece* p, COLOR) {
	// Code for hardware LED
}
*/

bool check_legal(piece *p) {
	return (p.type == NULL);
}

void reset_board(piece *board) {

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
		}
	}

}

void move_piece(piece *board, piece *p, int x, int y) {
	if (p.firstMove == true)
		p.firstMove == false

	board[x][y] = p;

	p.type = NONE;
	p.color = NONE;
}
