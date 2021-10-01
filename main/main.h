// Author:  Alex Gonzalez
// Date:    23 September 2021
// Project: Smart Chess Board

// Piece
typedef enum piece_type { NONE, PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING } piece_type;

typedef enum piece_color { NONE, BLACK, WHITE } piece_color;

typedef struct _piece {
	piece_type type;
	piece_color color;
};

typedef struct _square {
	struct _piece piece;
	char row;
	int column;
};

