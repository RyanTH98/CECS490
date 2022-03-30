#include "chess.h"
#include <stdio.h>

using namespace Chess;

extern Board board;


/* Proposed Change
using namespace std;
 

// Base Piece
BasePiece::BasePiece(){}
BasePiece::~BasePiece(){}

void BasePiece::setPosition( Position pos ) { this -> pos = pos; }
Color BasePiece::getColor(){ return color; }
Position BasePiece::getPosition(){ return pos; }
vector<Position> BasePiece::getLegalMoves(){ return legalMoves; }

// Square
Square::Square(){ piece = NULL; }
Square::~Square(){}

void Square::clearSquare() { piece = NULL; }
void Square::setPiece(BasePiece* piece){ this->piece = piece; }
BasePiece* Square::getPiece() { return piece; }

Board::Board(){ initBoard(); }
Board::~Board(){ }
 
bool BasePiece::validateMove(Position dest){
    for(int i = 0; i < 24; i++) {
        if(dest.x == legalMoves[i].x && dest.y == legalMoves[i].y)
            return true;
    }
 
    return false;
}
*/


// Definitions for BasePiece Class
BasePiece::BasePiece(){

}

BasePiece::~BasePiece(){
    //printf("Deconstructing BasePiece at %d, %d\n", pos.x, pos.y);
}

std::vector<Position> BasePiece::getLegalMoves(){
    return legalMoves;
}

bool BasePiece::validateMove(Position dest){
    for(int i = 0; i < 24; i++){
        if(dest.x == legalMoves[i].x && dest.y == legalMoves[i].y){
            return true;
        }
    }
    return false;
}



Position BasePiece::getPosition(){
    return pos;
}

void BasePiece::setPosition(Position pos){
    this->pos = pos;
}

Color BasePiece::getColor(){
    return color;
}

//definitions for Square class
Square::Square(){
    piece = NULL;
}

Square::~Square(){
    //printf("Deconstructing Square");
}

void Square::setPiece(BasePiece* piece){
    this->piece = piece;
}

BasePiece* Square::getPiece(){
    return piece;
}

void Square::clearSquare(){
    piece = NULL;
}


// Definitions for the Board Class
Board::Board(){
    initBoard();
}

Board::~Board(){
    //printf("Deconstructing Board\n");
}

/* Function:    initBoard
 * Arguments:   None
 * Description: Places each player's starting pieces on their appropriate squares
 */
void Board::initBoard(){
    Position pos;

    pos = {0,0};
    board[pos.y][pos.x].setPiece(new Rook(White, pos));
    pos = {1,0};
    board[pos.y][pos.x].setPiece(new Knight(White, pos));
    pos = {2,0};
    board[pos.y][pos.x].setPiece(new Bishop(White, pos));
    pos = {3,0};
    board[pos.y][pos.x].setPiece(new King(White, pos));
    pos = {4,0};
    board[pos.y][pos.x].setPiece(new Queen(White, pos));
    pos = {5,0};
    board[pos.y][pos.x].setPiece(new Bishop(White, pos));
    pos = {6,0};
    board[pos.y][pos.x].setPiece(new Knight(White, pos));
    pos = {7,0};
    board[pos.y][pos.x].setPiece(new Rook(White, pos));

    for(int i = 0; i < 8; i++){
        pos = {i,1};
        board[pos.y][pos.x].setPiece(new Pawn(White, pos));
    }

    pos = {0,7};
    board[pos.y][pos.x].setPiece(new Rook(Black, pos));
    pos = {1,7};
    board[pos.y][pos.x].setPiece(new Knight(Black, pos));
    pos = {2,7};
    board[pos.y][pos.x].setPiece(new Bishop(Black, pos));
    pos = {3,7};
    board[pos.y][pos.x].setPiece(new Queen(Black, pos));
    pos = {4,7};
    board[pos.y][pos.x].setPiece(new King(Black, pos));
    pos = {5,7};
    board[pos.y][pos.x].setPiece(new Bishop(Black, pos));
    pos = {6,7};
    board[pos.y][pos.x].setPiece(new Knight(Black, pos));
    pos = {7,7};
    board[pos.y][pos.x].setPiece(new Rook(Black, pos));

    for(int i = 0; i < 8; i++){
        pos = {i,6};
        board[pos.y][pos.x].setPiece(new Pawn(Black, pos));
    }
}


/* Function:    isInBounds
 * Arguments:   Position to check
 * Description: Returns whether or not the position is within the matrix
 */
bool Board::isInBounds(Position pos){
    if(pos.x > 7 || pos.x < 0){
        return false;
    }
    if(pos.y > 7 || pos.y < 0){
        return false;
    }
    return true;
}


/* Function:    movePiece
 * Arguments:   Original position, Destination position
 * Description: Moves a piece from one square to another
 */
bool Board::movePiece(Position origin, Position dest){
    if(getPiece(origin)->validateMove(dest)){
        board[dest.y][dest.x].clearSquare();
        board[dest.y][dest.x].setPiece(getPiece(origin));
        board[origin.y][origin.x].clearSquare();
        return true;
    }
    else{
        return false;
    }
}


/* Function:    getPiece
 * Arguments:   (x, y) pair of coordinates
 * Description: Returns the piece object at the given position
 */
BasePiece* Board::getPiece(Position piecePos){
    return board[piecePos.y][piecePos.x].getPiece();
}



// Definitions for Pawn Class
Pawn::Pawn(Color color, Position pos){
    this->color = color;
    this->pos = pos;
    doubleJump = true;
    enPassant = true;
}

Pawn::~Pawn(){
    printf("Deconstructing Pawn at %d, %d\n", pos.x, pos.y);
}


/* Function:    Pawn::populateLegalMoves
 * Arguments:   None
 * Description: Fills a pawn's object with the possible moves it can make
 */
void Pawn::populateLegalMoves(){
    legalMoves.clear();
    BasePiece* piece;
    Position testPos;
    int dir = (color == White)?1:-1;
    //if there is not a piece directly in front of the pawn -> it is legal move
    testPos = {pos.x, pos.y + 1*dir};
    piece = board.getPiece(testPos);
    if(piece == NULL && board.isInBounds(testPos)){
        legalMoves.push_back(testPos);
    }
    //if there is an enemy piece diagonal to the pawn it is a legal move
    testPos = {pos.x+1, pos.y + 1*dir};
    piece = board.getPiece(testPos);
    if(piece->getColor() != color && board.isInBounds(testPos)){
        legalMoves.push_back({pos.x+1, pos.y+1*dir});
    }
    testPos = {pos.x-1, pos.y + 1*dir};
    piece = board.getPiece(testPos);
    if(piece->getColor() != color && board.isInBounds(testPos)){
        legalMoves.push_back({pos.x-1, pos.y+1*dir});
    }
}




// Definitions for the Rook Class
Rook::Rook(Color color, Position pos){
    this->color = color;
    this->pos = pos;
}

Rook::~Rook(){
    printf("Deconstructing Rook at %d, %d\n", pos.x, pos.y);
}

/* Function:    Rook::populateLegalMoves
 * Arguments:   None
 * Description: Fills a rook's object with the possible moves it can make
 */
void Rook::populateLegalMoves(){
    legalMoves.clear();

    Position newPos;
    BasePiece* piece;
    bool left = true, right = true, up = true, down = true;
    
    for(int i = 0; i < 8; i++){
        //left
        if(left){
            newPos = {pos.x-i, pos.y};
            if(board.isInBounds(newPos)){
                piece = board.getPiece(newPos);
                
                //no piece
                if(piece == NULL){
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        legalMoves.push_back(newPos);
                        left = false;
                    }
                    else{ //my piece
                        left = false;
                    }
                }
            }
            else{ //out of bounds
                left = false;
            }
        }

        //right
        if(right){
            newPos = {pos.x+i, pos.y};
            if(board.isInBounds(newPos)){
                piece = board.getPiece(newPos);
                
                //no piece
                if(piece == NULL){
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        legalMoves.push_back(newPos);
                        right = false;
                    }
                    else{ //my piece
                        right = false;
                    }
                }
            }
            else{ //out of bounds
                right = false;
            }
        }

        //up
        if(up){
            newPos = {pos.x, pos.y+i};
            if(board.isInBounds(newPos)){
                piece = board.getPiece(newPos);
                
                //no piece
                if(piece == NULL){
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        legalMoves.push_back(newPos);
                        up = false;
                    }
                    else{ //my piece
                        up = false;
                    }
                }
            }
            else{ //out of bounds
                up = false;
            }
        }

        //down
        if(down){
            newPos = {pos.x+i, pos.y};
            if(board.isInBounds(newPos)){
                piece = board.getPiece(newPos);
                
                //no piece
                if(piece == NULL){
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        legalMoves.push_back(newPos);
                        down = false;
                    }
                    else{ //my piece
                        down = false;
                    }
                }
            }
            else{ //out of bounds
                down = false;
            }
        }
    }
}



// Definitions for the Bishop Class
Bishop::Bishop(Color color, Position pos){
    this->color = color;
    this->pos = pos;
}

Bishop::~Bishop(){
    printf("Deconstructing Bishop at %d, %d\n", pos.x, pos.y);
}


/* Function:    Bishop::populateLegalMoves
 * Arguments:   None
 * Description: Fills a bishop's object with the possible moves it can make
 */
void Bishop::populateLegalMoves(){
    legalMoves.clear();

    Position newPos;
    BasePiece* piece;
    bool ul = true, ur = true, dl = true, dr = true;

    for(int i = 0; i < 8; i++){
        //up-left
        if(ul){
            newPos = {pos.x-i, pos.y+i};
            if(board.isInBounds(newPos)){
                piece = board.getPiece(newPos);
                
                //no piece
                if(piece == NULL){
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        legalMoves.push_back(newPos);
                        ul = false;
                    }
                    else{ //my piece
                        ul = false;
                    }
                }
            }
            else{ //out of bounds
                ul = false;
            }
        }

        //up-right
        if(ur){
            newPos = {pos.x+i, pos.y+i};
            if(board.isInBounds(newPos)){
                piece = board.getPiece(newPos);
                
                //no piece
                if(piece == NULL){
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        legalMoves.push_back(newPos);
                        ur = false;
                    }
                    else{ //my piece
                        ur = false;
                    }
                }
            }
            else{ //out of bounds
                ur = false;
            }
        }

        //down-left
        if(dl){
            newPos = {pos.x-i, pos.y-i};
            if(board.isInBounds(newPos)){
                piece = board.getPiece(newPos);
                
                //no piece
                if(piece == NULL){
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        legalMoves.push_back(newPos);
                        dl = false;
                    }
                    else{ //my piece
                        dl = false;
                    }
                }
            }
            else{ //out of bounds
                dl = false;
            }
        }

        //down-right
        if(dr){
            newPos = {pos.x+i, pos.y-i};
            if(board.isInBounds(newPos)){
                piece = board.getPiece(newPos);
                
                //no piece
                if(piece == NULL){
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        legalMoves.push_back(newPos);
                        dr = false;
                    }
                    else{ //my piece
                        dr = false;
                    }
                }
            }
            else{ //out of bounds
                dr = false;
            }
        }
    }
}




// Definitions for the Knight Class
Knight::Knight(Color color, Position pos){
    this->color = color;
    this->pos = pos;
}

Knight::~Knight(){
    printf("Deconstructing Knight at %d, %d\n", pos.x, pos.y);
}

/* Function:    Knight::populateLegalMoves
 * Arguments:   None
 * Description: Fills a knight's object with the possible moves it can make
 */
void Knight::populateLegalMoves(){
    legalMoves.clear();

    Position newPos;
    BasePiece* piece;

    //up-left
    newPos = {pos.x-1, pos.y+2};
    if(board.isInBounds(newPos)){
        piece = board.getPiece(newPos);
        
        //no piece
        if(piece == NULL){
            legalMoves.push_back(newPos);
        }
        else{
            //piece
            if(piece->getColor() != color){ //enemy piece
                legalMoves.push_back(newPos);
            }
        }
    }

    //up-right
    newPos = {pos.x+1, pos.y+2};
    if(board.isInBounds(newPos)){
        piece = board.getPiece(newPos);
        
        //no piece
        if(piece == NULL){
            legalMoves.push_back(newPos);
        }
        else{
            //piece
            if(piece->getColor() != color){ //enemy piece
                legalMoves.push_back(newPos);
            }
        }
    }

    //down-left
    newPos = {pos.x-1, pos.y-2};
    if(board.isInBounds(newPos)){
        piece = board.getPiece(newPos);
        
        //no piece
        if(piece == NULL){
            legalMoves.push_back(newPos);
        }
        else{
            //piece
            if(piece->getColor() != color){ //enemy piece
                legalMoves.push_back(newPos);
            }
        }
    }

    //down-right
    newPos = {pos.x+1, pos.y-2};
    if(board.isInBounds(newPos)){
        piece = board.getPiece(newPos);
        
        //no piece
        if(piece == NULL){
            legalMoves.push_back(newPos);
        }
        else{
            //piece
            if(piece->getColor() != color){ //enemy piece
                legalMoves.push_back(newPos);
            }
        }
    }
}




// Definitions for the Queen Class
Queen::Queen(Color color, Position pos){
    this->color = color;
    this->pos = pos;
}

Queen::~Queen(){
    printf("Deconstructing Queen at %d, %d\n", pos.x, pos.y);
}


/* Function:    Queen::populateLegalMoves
 * Arguments:   None
 * Description: Fills a queen's object with the possible moves it can make
 */
void Queen::populateLegalMoves(){
    legalMoves.clear();

    BasePiece* bishop = new Bishop(color, pos);
    BasePiece* rook = new Rook(color, pos);

    bishop->populateLegalMoves();
    rook->populateLegalMoves();

    legalMoves = bishop->getLegalMoves();
    legalMoves.insert(rook->getLegalMoves().end(), rook->getLegalMoves().begin(), rook->getLegalMoves().end());

    bishop->~BasePiece();
    rook->~BasePiece();
}



// Definitions for the King Class
King::King(Color color, Position pos){
    this->color = color;
    this->pos = pos;
    check = false;
    leftCastle = false;
    rightCastle = false;
}

King::~King(){
    printf("Deconstructing King at %d, %d\n", pos.x, pos.y);
}


/* Function:    King::populateLegalMoves
 * Arguments:   None
 * Description: Fills a king's object with the possible moves it can make
 */
void King::populateLegalMoves(){
    legalMoves.clear();

    Position newPos;
    BasePiece* piece;

    //up-left
    newPos = {pos.x-1, pos.y+1};
    if(board.isInBounds(newPos)){
        piece = board.getPiece(newPos);
        
        //no piece
        if(piece == NULL){
            legalMoves.push_back(newPos);
        }
        else{
            //piece
            if(piece->getColor() != color){ //enemy piece
                legalMoves.push_back(newPos);
            }
        }
    }

    //up
    newPos = {pos.x, pos.y+1};
    if(board.isInBounds(newPos)){
        piece = board.getPiece(newPos);
        
        //no piece
        if(piece == NULL){
            legalMoves.push_back(newPos);
        }
        else{
            //piece
            if(piece->getColor() != color){ //enemy piece
                legalMoves.push_back(newPos);
            }
        }
    }

    //up-right
    newPos = {pos.x+1, pos.y+1};
    if(board.isInBounds(newPos)){
        piece = board.getPiece(newPos);
        
        //no piece
        if(piece == NULL){
            legalMoves.push_back(newPos);
        }
        else{
            //piece
            if(piece->getColor() != color){ //enemy piece
                legalMoves.push_back(newPos);
            }
        }
    }

    //right
    newPos = {pos.x+1, pos.y};
    if(board.isInBounds(newPos)){
        piece = board.getPiece(newPos);
        
        //no piece
        if(piece == NULL){
            legalMoves.push_back(newPos);
        }
        else{
            //piece
            if(piece->getColor() != color){ //enemy piece
                legalMoves.push_back(newPos);
            }
        }
    }

    //down-right
    newPos = {pos.x+1, pos.y-1};
    if(board.isInBounds(newPos)){
        piece = board.getPiece(newPos);
        
        //no piece
        if(piece == NULL){
            legalMoves.push_back(newPos);
        }
        else{
            //piece
            if(piece->getColor() != color){ //enemy piece
                legalMoves.push_back(newPos);
            }
        }
    }

    //down
    newPos = {pos.x, pos.y-1};
    if(board.isInBounds(newPos)){
        piece = board.getPiece(newPos);
        
        //no piece
        if(piece == NULL){
            legalMoves.push_back(newPos);
        }
        else{
            //piece
            if(piece->getColor() != color){ //enemy piece
                legalMoves.push_back(newPos);
            }
        }
    }

    //down-left
    newPos = {pos.x-1, pos.y-1};
    if(board.isInBounds(newPos)){
        piece = board.getPiece(newPos);
        
        //no piece
        if(piece == NULL){
            legalMoves.push_back(newPos);
        }
        else{
            //piece
            if(piece->getColor() != color){ //enemy piece
                legalMoves.push_back(newPos);
            }
        }
    }

    //left
    newPos = {pos.x-1, pos.y};
    if(board.isInBounds(newPos)){
        piece = board.getPiece(newPos);
        
        //no piece
        if(piece == NULL){
            legalMoves.push_back(newPos);
        }
        else{
            //piece
            if(piece->getColor() != color){ //enemy piece
                legalMoves.push_back(newPos);
            }
        }
    }
}
