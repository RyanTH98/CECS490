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
 *                  Benjamin Gillmore (bg) - bggillmore@gmail.com
 *
 *        Company:  California State University Long Beach
 *
 * =====================================================================================
 */

#include "main.h"
#include <stdio.h>
#include <stdlib.h>

/* String colors for console output */
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


int main(void) {
	int x, y;
	_space move[2];
	char name[20], yn;
	_piece board[8][8];

	reset_board(board);

	/* Begin Game */
	printf("Hello \nWould you like to play chess? (Y/N)\n> ");
        scanf(" %c", &yn);

        if((yn == 'y') | (yn == 'Y'))
                printf("\nPlease enter your name\n> ");
        else if((yn == 'n') | (yn == 'N')) {
                printf("Alright. See you later!");
                return 0;
        }
        else
                return 0;

        /* Clear buffer and get player's name */
        fgetc(stdin);
        fgets(name, 20, stdin);

        printf("\nHey, %sLet's begin!\n\n", name);
        printf("Please use the following format: [Letter][Number].\nExamples: A0, e7, C6\n\n");
        print_board(board);
	while(true){
        	get_move(move);

        	move_piece(board, move);
        	//printf("Moving %d%d to %d%d", move[0], move[1], move[2], move[3]);
        	print_board(board);
	}
	return 0;
}

/*  Function: move_piece
 *
 *          Inputs:   2D Array of Chess Board,
 *                    Piece to be moved,
 *                    Index of destination row,
 *                    Index of destination column
 *          Outputs:  None
 */
void move_piece(_piece(*board)[8], _space move[2]) {
        bool legal = false;
	int row1, row2, column1, column2;
	row1 = move[0]/8;
	column1 = move[0]%8;
	row2 = move[1]/8;
	column2 = move[1]%8;
	for(int i = 0; i < 26; i++){
		//printf("%d == %d => %d\n", board[row1][column1].legal[i], move[1], board[row1][column1].legal[i] == move[1]);
		if(board[row1][column1].legal[i] == move[1]){
			legal = true;
		}
	}
	if(legal){
		board[row2][column2] = board[row1][column1];
		board[row2][column2].position = move[1];
		board[row1][column1].type = NO_PIECE;
		board[row1][column1].color = NO_COLOR;
		update_legal(board);
	}
	else{
		printf("Not a legal move\n");
	}
}



/*	Function: 	get_move
 *	Inputs: 	enum space[2] move index1 to index2
 *	Outputs:	None
 *
 */
void get_move(_space move[2]) {
        char new_move[6];

        printf("\n\nPlease enter a move\n> ");
        fgets(new_move, 6, stdin);


        
	for (int i = 0; i < 4; i++) {
		printf("--------> %d\n", to_int(new_move[i]));
                new_move[i] = to_int(new_move[i]);
		if(new_move[i] == -1){
			//if you wish to make it repetedly ask for a valid answer make it recursive
			//im close but its printing multiple times, (fgets buffer?)
			//get_move(move);
			return;
		}
        }
	move[0] = new_move[0] + new_move[1]*8;
	move[1] = new_move[2] + new_move[3]*8;

        printf("Moving %d to %d\n\n", move[0], move[1]);
}

/*  Function: print_board

	Inputs:   2D Array of Chess Board,

	Outputs:  None
*/
void print_board(_piece(*board)[8]) {
	int i, j;

	printf("%s---------------------------------\n", KMAG);
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			printf("%s|", KMAG);
			printf("%s", (board[i][j].color == BLACK) ? KGRN : KBLU);
			switch (board[i][j].type) {
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


void printLegalMoves(_piece* p){
	printf("Legal moves for piece on square %d: ", p->position);
	for(int i=0; i<sizeof(p->legal)/sizeof(p->legal[0]);i++){
		if(p->legal[i] != -1){
			printf("%d, ", p->legal[i]);
		}
	}
	printf("\n");
}


/*  Function: reset_legal

	Inputs:   2D Array of Chess Board,

	Outputs:  None
*/
void reset_board(_piece(*board)[8]) {

	/* Assign color of all White pieces
	   Set default direction to moving downward */
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j].color = WHITE;
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


	/* Assign color of all Black pieces
       Set default direction to moving upward */
	for (int i = 6; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				board[i][j].color = BLACK;
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
			board[i][j].position = (i*8)+j;

			if(i >= 2 && i <= 5){
				board[i][j].type = NO_PIECE;
				board[i][j].color = NO_COLOR;
			}
		}
	}

	/*get legal moves for every piece on the board*/
	update_legal(board);

}
int to_int(char c) {
	int n = (int)c;
        /* Lowercase Letters */
	// Maps a-h -> 0-7
        if ((n >= 97) && (n <= 104))
                return (n - 97);
        // Maps A-H -> 0-7
	/* Uppercase Letters */
        else if ((n >= 65) && (n <= 72))
                return (n - 65);
        /* Numbers 0 - 9 */
	// Map char 1-8 to int 0-7
        else if ((n > 48) && (n < 57))
                return (n - 49);
	else
		return -1;
}

/*
 *
 *
 */
void update_legal(_piece(*board)[8]){
	_piece* p;
	for(int i = 0; i < 64; i++){
		p = &board[i/8][i%8];
		//clear old moves
		for(int l = 0; l < sizeof(p->legal)/sizeof(p->legal[0]); l++){
			p->legal[l] = -1;
		}

		switch(p->type){
			case NO_PIECE:
				break;
			case PAWN:
				get_moves_pawn(board, p);
				break;
			case BISHOP:
				//get_moves_bishop(board, &p);
				break;
			case KNIGHT:
				//get_moves_knight(board, &p);
				break;
			case ROOK:
				get_moves_rook(board, p);
				break;
			case QUEEN:
				//get_moves_queen(board, &p);
				break;
			case KING:
				//get_moves_king(board, &p);
				break;
			default:
				break;

		}
		printLegalMoves(p);
	}
}



void get_moves_pawn(_piece(*board)[8], _piece* p){
	//for now im leaving out enpasant and queening
	int row, column, x, y, index;
	row = p->position/8;
	column = p->position%8;
	index = 0;
	switch(p->color){
		case BLACK:
			if(row == 6){ //first move
				if(board[row-2][column].type == NO_PIECE){ //check two ahead for piece
					p->legal[index] = ((row-2)*8+column);
					index++;
				}
			}
			if(board[row-1][column].type == NO_PIECE){ //check one ahead for piece
				p->legal[index] = ((row-1)*8+column);
				index++;
			}
			if( (board[row-1][column-1].type != NO_PIECE) && (board[row-1][column-1].color == WHITE) ){ //check for piece to take to the left
				p->legal[index] = ((row-1)*8+(column-1));
				index++;	
			}
			if( (board[row-1][column+1].type != NO_PIECE) && (board[row-1][column+1].color == WHITE) ){ //check for piece to take to the right
				p->legal[index] = ((row-1)*8+(column+1));
				index++;	
			}
			break;
		case WHITE:
			if(row == 1){ //first move
				if(board[row+2][column].type == NO_PIECE){ //check two ahead for piece
					p->legal[index] = ((row+2)*8+column);
					index++;
				}
			}
			if(board[row+1][column].type == NO_PIECE){ //check one ahead for piece
				printf("checkingforpawn");
				p->legal[index] = ((row+1)*8+column);
				index++;
			}
			if( (board[row+1][column-1].type != NO_PIECE) && (board[row+1][column-1].color == BLACK) ){ //check for piece to take in upper left
				p->legal[index] = ((row+1)*8+(column-1));
				index++;	
			}
			if( (board[row+1][column+1].type != NO_PIECE) && (board[row+1][column+1].color == BLACK) ){ //check for piece to take in upper right
				p->legal[index] = ((row+1)*8+(column+1));
				index++;	
			}
			break;
	}


}



void get_moves_rook(_piece(*board)[8], _piece* p){
	int row, column, x, y, index;
	bool hit;

	row = p->position/8;
	column = p->position%8;
	index = 0;

	//for each direction a rook can move
	// add the legal sqares to its legal member
	for(int dir = 0; dir < 4; dir++){
		x = 1;
		y = 1;
		hit = false;
		switch(dir){
			case 0: //up
				while((row+y < 8) && (hit == false)){
					if(board[row+y][column].color != p->color){
						p->legal[index] = ((row+y)*8+column);
						index++;
					}
					if(board[row+y][column].type != NO_PIECE){
						hit = true;
					}
					y++;
				}
				break;
			case 1: //down
				while((row-y >= 0) && (hit == false)){
					if(board[row-y][column].color != p->color){
						p->legal[index] = ((row-y)*8+column);
						index++;
					}
					if(board[row-y][column].type != NO_PIECE){
						hit = true;
					}
					y++;
				}
				break;
			case 2: //right
				while((column+x < 8) && (hit == false)){
					if(board[row][column+x].color != p->color){
						p->legal[index] = (row*8)+(column+x);
						index++;
					}
					if(board[row][column+x].type != NO_PIECE){
						hit = true;
					}
					x++;
				}
				break;
			case 3: //left 
				while((column-x >= 0) && (hit == false)){
					if(board[row][column-x].color != p->color){
						p->legal[index] = (row*8)+(column-x);
						index++;
					}
					if(board[row][column-x].type != NO_PIECE){
						hit = true;
					}
					x++;
				}
				break;
		}
	}	
}
