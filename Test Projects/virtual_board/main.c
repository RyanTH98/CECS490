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
			/* If the pawn HAS NOT moved yet, it can move forward by one OR two spaces, OR forward and to either side (if there is a capturable piece)
			   If the pawn HAS moved, it can move forward by one space, OR forward and to either side */
			
			
			if (p->direction == 'D') { // Piece moving Down
				if (check_legal(board[p->row - 1][p->column]))     // Down
					// illuminate(&p, white);
				if (check_legal(board[p->row - 1][p->column - 1])) // Down & Left
					// illuminate(&p, white);
				if (check_legal(board[p->row - 1][p->column + 1])) // Down & Right
					// illuminate(&p, white);
				if (p->firstMove == true)
					if (check_legal(board[p->row - 2][p->column])) // Down by two
						// illuminate(&p, white);
		    } else if(p->direction == 'U') { // Piece moving Up
				if (check_legal(board[p->row + 1][p->column]))	   // Up
					// illuminate(&p, white);
				if (check_legal(board[p->row + 1][p->column - 1])) // Up & Left
					// illuminate(&p, white);
				if (check_legal(board[p->row + 1][p->column + 1])) // Up & Right
					// illuminate(&p, white);
				if (p->firstMove == true)
					if (check_legal(board[p->row + 2][p->column])) // Up by two
						// illuminate(&p, white);
			} else {}
			break;
		case BISHOP:
			/* Diagonally, Down & Right */
			for (int i = p->row; i < 8; i--) {
				for (int j = p->column; j < 8; j++) {
					if (check_legal(board[i][j])) {
						// illuminate(&p, white);
					} else {
						// illuminate(&p, red);
						break;
					}
				}
			}

			/* Diagonally, Down & Left */
			for (int i = p->row; i < 8; i--) {	// Check Down, diagonally, until a piece is detected or the index reaches the edge of the board
				for (int j = p->column; j >= 0; j--) {
					if (check_legal(board[i][j])) {
						// illuminate(&p, white);
					} else {
						// illuminate(&p, red);
						break;
					}
				}
			}

			/* Diagonally, Up & Right */
			for (int i = p->row; i >= 0; i--) {	// Check Down, diagonally, until a piece is detected or the index reaches the edge of the board
				for (int j = p->column; j > 8; j--) {
					if (check_legal(board[i][j])) {
						// illuminate(&p, white);
					} else {
						// illuminate(&p, red);
						break;
					}
				}
			}

			/* Diagonally, Up & Left */
			for (int i = p->row; i >= 0; i--) {	// Check Down, diagonally, until a piece is detected or the index reaches the edge of the board
				for (int j = p->column; j <= 0; j--) {
					if (check_legal(board[i][j])) {
						// illuminate(&p, white);
					} else {
						// illuminate(&p, red);
						break;
					}
				}
			}
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
