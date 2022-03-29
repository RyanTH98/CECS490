#include "chess.h"

using namespace Chess;

//definitions for BasePiece Class
BasePiece::BasePiece(){

}

BasePiece::~BasePiece(){
    printf("Deconstructing BasePiece at %d, %d\n", pos.x, pos.y);
}

Position* BasePiece::getLegalMoves(){
    return legalMoves;
}

bool BasePiece::validateMove(Position dest){
    for(int i = 0; i < 24; i++){
        if(dest == legalMoves[i]){
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
    printf("Deconstructing Square");
}

Square::setPiece(BasePiece* piece){
    this->piece = piece;
}

BasePiece* Square::getPiece(){
    return piece;
}

void Square::clearSquare(){
    piece = NULL;
}


//definitions for Board Class
Board::Board(){
    initBoard();
}

Board::~Board(){
    printf("Deconstructing Board\n");
}

void Board::initBoard(){
    Position pos;

    pos = {0,0};
    board[pos.y][pos.x].SetPiece(new Rook(White, pos));
    pos = {1,0};
    board[pos.y][pos.x].SetPiece(new Knight(White, pos));
    pos = {2,0};
    board[pos.y][pos.x].SetPiece(new Bishop(White, pos));
    pos = {3,0};
    board[pos.y][pos.x].SetPiece(new King(White, pos));
    pos = {4,0};
    board[pos.y][pos.x].SetPiece(new Queen(White, pos));
    pos = {5,0};
    board[pos.y][pos.x].SetPiece(new Bishop(White, pos));
    pos = {6,0};
    board[pos.y][pos.x].SetPiece(new Knight(White, pos));
    pos = {7,0};
    board[pos.y][pos.x].SetPiece(new Rook(White, pos));

    for(int i = 0; i < 8; i++){
        pos = {i,1};
        board[pos.y][pos.x].SetPiece(new Pawn(White, pos));
    }

    pos = {0,7};
    board[pos.y][pos.x].SetPiece(new Rook(Black, pos));
    pos = {1,7};
    board[pos.y][pos.x].SetPiece(new Knight(Black, pos));
    pos = {2,7};
    board[pos.y][pos.x].SetPiece(new Bishop(Black, pos));
    pos = {3,7};
    board[pos.y][pos.x].SetPiece(new Queen(Black, pos));
    pos = {4,7};
    board[pos.y][pos.x].SetPiece(new King(Black, pos));
    pos = {5,7};
    board[pos.y][pos.x].SetPiece(new Bishop(Black, pos));
    pos = {6,7};
    board[pos.y][pos.x].SetPiece(new Knight(Black, pos));
    pos = {7,7};
    board[pos.y][pos.x].SetPiece(new Rook(Black, pos));

    for(int i = 0; i < 8; i++){
        pos = {i,6};
        board[pos.y][pos.x].SetPiece(new Pawn(Black, pos));
    }
}

bool Board::isInBounds(Position pos){
    if(pos.x > 7 || pos.x < 0){
        return false;
    }
    if(pos.y > 7 || pos.y < 0){
        return false;
    }
    return true
}

bool Board::movePiece(Position origin, Position dest){
    if(getPiece(origin)->validateMove(dest)){
        board[dest.y][dest.x].clearSquare();
        board[dest.y][dest.x].SetPiece(board.getPiece(origin));
        board[origin.y][origin.x].clearSquare();
        return true;
    }
    else{
        return false;
    }
}

BasePiece* Board::getPiece(Position piecePos){
    return board[piecePos.y][piecePos.x].getPiece();
}


//definitions for Pawn Class
Pawn::Pawn(Color color, Position pos){
    this->color = color;
    this->pos = pos;
    doubleJump = true;
    enPassant = true;
}

Pawn::~Pawn(){
    printf("Deconstructing Pawn at %d, %d\n", pos.x, pos.y);
}

void Pawn::populateLegalMoves(){
    legalMoves.clear();
    int dir = (color == White)?1:-1;
    //if there is not a piece directly in front of the pawn -> it is legal move
    if(board[pos.y + 1*dir][pos.x].getPiece() == NULL && board[pos.y + 1*dir][pos.x].isInBounds()){
        legalMoves.push_back({pos.x, pos.y+1*dir});
    }
    //if there is an enemy piece diagonal to the pawn it is a legal move
    if(board[pos.y + 1*dir][pos.x+1].getPiece().color != color && board[pos.y + 1*dir][pos.x+1].isInBounds()){
        legalMoves.push_back({pos.x+1, pos.y+1*dir});
    }
    if(board[pos.y + 1*dir][pos.x-1].getPiece().color != color && board[pos.y + 1*dir][pos.x-1].isInBounds()){
        legalMoves.push_back({pos.x-1, pos.y+1*dir});
    }
}

//definitions for Rook Class
Rook::Rook(Color color, Position pos){
    this->color = color;
    this->pos = pos;
}

Rook::~Rook(){
    printf("Deconstructing Rook at %d, %d\n", pos.x, pos.y);
}

void Rook::populateLegalMoves(){
    legalMoves.clear();

    Position newPos;
    BasePiece* piece;
    bool left = true, right = true, up = true, down = true;
    
    for(int i = 0; i < 8; i++){
        //left
        if(left){
            newPos = {pos.x-i, pos.y};
            if(board[newPos.y][newPos.x].isInBounds()){
                piece = board[newPos.y][newPos.x].getPiece();
                
                //no piece
                if(piece == NULL){
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->color != color){ //enemy piece
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
            if(board[newPos.y][newPos.x].isInBounds()){
                piece = board[newPos.y][newPos.x].getPiece();
                
                //no piece
                if(piece == NULL){
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->color != color){ //enemy piece
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
            if(board[newPos.y][newPos.x].isInBounds()){
                piece = board[newPos.y][newPos.x].getPiece();
                
                //no piece
                if(piece == NULL){
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->color != color){ //enemy piece
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
            if(board[newPos.y][newPos.x].isInBounds()){
                piece = board[newPos.y][newPos.x].getPiece();
                
                //no piece
                if(piece == NULL){
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->color != color){ //enemy piece
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

//definitions for Bishop Class
Bishop::Bishop(Color color, Position pos){
    this->color = color;
    this->pos = pos;
}

Bishop::~Bishop(){
    printf("Deconstructing Bishop at %d, %d\n", pos.x, pos.y);
}

void Bishop::populateLegalMoves(){
    legalMoves.clear();

    Position newPos;
    BasePiece* piece;
    bool ul = true, ur = true, dl = true, dr = true;

    for(int i = 0; i < 8; i++){
        //up-left
        if(ul){
            newPos = {pos.x-i, pos.y+i};
            if(board[newPos.y][newPos.x].isInBounds()){
                piece = board[newPos.y][newPos.x].getPiece();
                
                //no piece
                if(piece == NULL){
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->color != color){ //enemy piece
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

        //ur
        if(ur){
            newPos = {pos.x+i, pos.y+i};
            if(board[newPos.y][newPos.x].isInBounds()){
                piece = board[newPos.y][newPos.x].getPiece();
                
                //no piece
                if(piece == NULL){
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->color != color){ //enemy piece
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

        //dl
        if(dl){
            newPos = {pos.x-i, pos.y-i};
            if(board[newPos.y][newPos.x].isInBounds()){
                piece = board[newPos.y][newPos.x].getPiece();
                
                //no piece
                if(piece == NULL){
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->color != color){ //enemy piece
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

        //dr
        if(dr){
            newPos = {pos.x+i, pos.y-i};
            if(board[newPos.y][newPos.x].isInBounds()){
                piece = board[newPos.y][newPos.x].getPiece();
                
                //no piece
                if(piece == NULL){
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->color != color){ //enemy piece
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

//definitions for Knight Class
Knight::Knight(Color color, Position pos){
    this->color = color;
    this->pos = pos;
}

Knight::~Knight(){
    printf("Deconstructing Knight at %d, %d\n", pos.x, pos.y);
}

void Knight::populateLegalMoves(){
    
}

//definitions for Queen Class
Queen::Queen(Color color, Position pos){
    this->color = color;
    this->pos = pos;
}

Queen::~Queen(){
    printf("Deconstructing Queen at %d, %d\n", pos.x, pos.y);
}

void Queen::populateLegalMoves(){
    legalMoves.clear();

    BasePiece* bishop = new Bishop(color, pos);
    BasePiece* rook = new Rook(color, pos);

    bishop->populateLegalMoves();
    rook->populateLegalMoves();
}

//definitions for King Class
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

void King::populateLegalMoves(){
    
}
