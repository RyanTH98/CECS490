// Author:  Alex Gonzalez
// Project: Smart Chess Board

// Piece
typedef enum type { NONE, PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING } type;

typedef enum color { NONE, BLACK, WHITE } color;

typedef struct _piece {
	type type = NONE;
	color color = NONE;
} piece;



int main(void) {

	piece board[7][7];

	reset_board(board);

	printf("Test");

	return 0;
}


void get_moves(piece board, piece p) {
	switch(p) {
		case NONE:
			break;
		case PAWN:
			if(p.color == BLACK) {
				// Return x and y of spaces below - straight and diagonally
			} else {
				// return x and y of spaces above - straight and diagonally
			}
			break;
		case BISHOP:
			break;
		case KNIGHT:
			break;
		case ROOK:
			break;
		case QUEEN:
			break;
		case KING:
			break;
		default:
	}
}

void reset_board(piece board) {

	/* Make upper piece black */
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j].color = BLACK;

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


	/* Make lower pieces white */
	for (int i = 6; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				board[i][j].color = WHITE;

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

}

void move_piece(piece board, piece p, int x, int y) {
	board[x][y] = p;

	p.type = NONE;
	p.color = NONE;
}
