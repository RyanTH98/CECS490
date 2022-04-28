#include "chess.h"


#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


using namespace Chess;
//extern Board board;

// Definitions for BasePiece Class
BasePiece::BasePiece(){
    firstMove = true;
}

BasePiece::~BasePiece(){
#ifdef DEBUG
    printf("Deconstructing BasePiece at %d, %d\n", pos.x, pos.y);
#endif
}

std::vector<Position> BasePiece::getLegalMoves(){
    std::vector<Position> lMoves, tempMoves;

    if(!legalMoves.empty()){
        tempMoves = legalMoves;
        for(Position move : tempMoves){
            //legalMovesPrint();
            if(board->checkForCheck(color, pos, move) == false){
                lMoves.push_back(move);
            }
        }
    }
    
    return lMoves;
}

bool BasePiece::getFirstMove(){
    #ifdef DEBUG
        printf("Returning firstMove: %d\n", firstMove);
    #endif
    return firstMove;
}

void BasePiece::setFirstMove(bool firstMove){
    this->firstMove = firstMove;
    #ifdef DEBUG
        printf("Setting firstMove to %d\n", this->firstMove);
    #endif
}



bool BasePiece::validateMove(Position dest){
    for(Position move : legalMoves){
        #ifdef DEBUG
            printf("Checking {%2d, %2d} against {%2d, %2d} with result: %d\n", 
                dest.x, dest.y, move.x, move.y, (dest.x == move.x && dest.y == move.y));
        #endif
        if(dest.x == move.x && dest.y == move.y){
            if(board->checkForCheck(color, pos, dest) == false){
                return true;
            }
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

Type BasePiece::getType(){
    return type;
}

void BasePiece::legalMovesPrint(){
    for(Position m : legalMoves){
        printf("{%2d %2d}, ", m.x, m.y);
    }
    printf("\n");
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
    if(piece == NULL){
        return NULL;
    }
    else{
        return piece;
    }
}

void Square::clearSquare(){
    piece = NULL;
}


// Definitions for the Board Class
Board::Board(){
    whiteInCheck = false;
    blackInCheck = false;
    initBoard();
}

Board::~Board(){
#ifdef DEBUG
    printf("Deconstructing Board\n");
#endif
}

/* Function:    initBoard
 * Arguments:   None
 * Description: Places each player's starting pieces on their appropriate squares
 */
void Board::initBoard(){
    Position pos;

    pos = {0,0};
    board[pos.x][pos.y].setPiece(new Rook(White, pos, this));
    pos = {1,0};
    board[pos.x][pos.y].setPiece(new Knight(White, pos, this));
    pos = {2,0};
    board[pos.x][pos.y].setPiece(new Bishop(White, pos, this));
    pos = {3,0};
    board[pos.x][pos.y].setPiece(new King(White, pos, this));
    whiteKingLocation = pos;
    pos = {4,0};
    board[pos.x][pos.y].setPiece(new Queen(White, pos, this));
    pos = {5,0};
    board[pos.x][pos.y].setPiece(new Bishop(White, pos, this));
    pos = {6,0};
    board[pos.x][pos.y].setPiece(new Knight(White, pos, this));
    pos = {7,0};
    board[pos.x][pos.y].setPiece(new Rook(White, pos, this));

    for(int i = 0; i < 8; i++){
        pos = {i,1};
        board[pos.x][pos.y].setPiece(new Pawn(White, pos, this));
    }

    pos = {0,7};
    board[pos.x][pos.y].setPiece(new Rook(Black, pos, this));
    pos = {1,7};
    board[pos.x][pos.y].setPiece(new Knight(Black, pos, this));
    pos = {2,7};
    board[pos.x][pos.y].setPiece(new Bishop(Black, pos, this));
    pos = {3,7};
    board[pos.x][pos.y].setPiece(new King(Black, pos, this));
    blackKingLocation = pos;
    pos = {4,7};
    board[pos.x][pos.y].setPiece(new Queen(Black, pos, this));
    pos = {5,7};
    board[pos.x][pos.y].setPiece(new Bishop(Black, pos, this));
    pos = {6,7};
    board[pos.x][pos.y].setPiece(new Knight(Black, pos, this));
    pos = {7,7};
    board[pos.x][pos.y].setPiece(new Rook(Black, pos, this));

    for(int i = 0; i < 8; i++){
        pos = {i,6};
        board[pos.x][pos.y].setPiece(new Pawn(Black, pos, this));
    }

    #ifdef DEBUG
        printf("All pieces have been placed, now calling populateAllLegalMoves()\n");
    #endif
    populateAllLegalMoves();

    printBoard();
}

void Board::populateAllLegalMoves(){
    Position pos;
    BasePiece* piece;
    
    for(int i = 0; i < 64; i++){
        pos = {i%8, i/8};
        piece = getPiece({pos.x, pos.y});

        if(piece != NULL){
            #ifdef DEBUG
                //printf("Populating legal moves for piece at column: %d, row: %d\n", pos.x, pos.y);
            #endif
            piece->populateLegalMoves();
        }
        else{
            #ifdef DEBUG
                //printf("Piece is a NULL piece\n");
            #endif
        }
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
    BasePiece* piece = getPiece(origin);
    Chess::Color pieceColor = piece->getColor();

    #ifdef DEBUG
        printf("Moving piece at {%2d, %2d} to {%2d, %2d} is legal(1) move: %d--------\n", origin.x, origin.y, dest.x, dest.y, piece->validateMove(dest));
        piece->legalMovesPrint();
    #endif
    if(piece->validateMove(dest)){
        if(piece->getFirstMove() == true){
            piece->setFirstMove(false);
        }
        

        // Queening
        #define DEBUG_A
        #ifdef 	DEBUG_A	

        piece->setPosition(dest);

        int row = dest.y;
        if(piece->getType() == PawnType){
            if ((row == 0 && pieceColor == Black) || (row == 7 && pieceColor == White)) {
                //not very memory safe...
                piece = new Queen(pieceColor, piece->getPosition(), this);
            }
        }

        #endif
    
        board[dest.x][dest.y].clearSquare();
        board[dest.x][dest.y].setPiece(piece);
        board[origin.x][origin.y].clearSquare();
    
        //Castling
        if(piece->getType() == KingType){
            BasePiece* rookPiece;
            //moving two squares left -> castle left
            if(origin.x - dest.x == 2){
                //get rook on left
                rookPiece = getPiece({0,0});
                //move rook to needed square
                board[2][0].clearSquare();
                board[2][0].setPiece(rookPiece);
                board[0][0].clearSquare();
            }
            //moving two squares right -> castle left
            else if(dest.x - origin.x == 2){
                //get rook on right
                rookPiece = getPiece({7,0});
                //move rook to needed square
                board[4][0].clearSquare();
                board[4][0].setPiece(rookPiece);
                board[0][7].clearSquare();
            }
        }

        printBoard();
        
        populateAllLegalMoves();
        return true;
    }
    else{
        return false;
    }
}

void Board::printBoard(){
    BasePiece* piece;
    int row, column;
    printf("\033[2J"); //clear screen
    printf("Printing Board as seen by Chess::Board::printBoard\n");

    printf("%s---------------------------------\n", KMAG);
    for(int i = 0; i < 64; i++){
        row = 7 - (i/8);
        column = i%8;
        piece = board[column][row].getPiece();

        if(column == 0 && i != 0){
            printf("%s|\n", KMAG);
            printf("%s---------------------------------\n", KMAG);
        }

        printf("%s|", KMAG);

        if(piece != NULL){
            printf("%s", (piece->getColor() == White) ? KGRN : KBLU);
            switch(piece->getType()){
                case PawnType:
                    printf(" P ");
                    break;
                case RookType:
                    printf(" R ");
                    break;
                case BishopType:
                    printf(" B ");
                    break;
                case KnightType:
                    printf(" k ");
                    break;
                case KingType:
                    printf(" K ");
                    break;
                case QueenType:
                    printf(" Q ");
                    break;
            }
        }
        else{
            printf("   ");
        }
        
    }
    printf("%s|\n", KMAG);
    printf("%s---------------------------------\n", KMAG);
    printf("\n\n\n");
    printf("%s", KWHT);
}

/* Function:    getPiece
 * Arguments:   (x, y) pair of coordinates
 * Description: Returns the piece object at the given position
 */
BasePiece* Board::getPiece(Position piecePos){
    BasePiece* piece = board[piecePos.x][piecePos.y].getPiece();
    #ifdef DEBUG
        //printf("Getting piece at piece at collum: %d, row: %d\n", piecePos.x, piecePos.y);
    #endif
    if(piece == NULL){
        #ifdef DEBUG
            //printf("Piece is NULL\n");
        #endif
        return NULL;
    }
    else{
        #ifdef DEBUG
            //printf("Piece is a Valid piece\n");
        #endif
        return piece;
    }
}

std::vector<int> Board::getActiveSquares(){
    std::vector<int> activeSquares;

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(board[j][i].getPiece() != NULL){
                activeSquares.push_back(1);
            }
            else{
                activeSquares.push_back(0);
            }
        }
    }

    return activeSquares;
}

bool Board::isInCheck(Color color){
    return (color == White) ? whiteInCheck : blackInCheck;
}

/*
 *  Function : evaluateGameOver()
 *  Input: RGBColor of player to evaluate
 *  Output: Boolean value representing if game has reached its end.
 *  Game Over Conditions:
 *      1) Player has no remaining moves.
 *      2) Only two kings on board
 */
bool Board::evaluateGameOver(Color color) {
    bool onlyKings = true;
    bool noMoves = true;
    BasePiece* piece;
    std::vector<int> activeSquares;

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            piece = getPiece({i, j});
            
            if(piece != NULL) {
                if(color == piece->getColor()){
                    if (!piece->getLegalMoves().empty()) {
                        noMoves = false;
                    }
                }
                if (piece->getType() != KingType) {
                    onlyKings = false;
                }
            }    
        }
    }

    return onlyKings || noMoves;
}

void Board::setCheck(Color color, bool check){
    if(color == White){
        whiteInCheck = check;
    }
    else if(color == Black){
        blackInCheck = check;
    }
}

bool Board::checkForCheck(Color color, Position origin, Position dest){    
    BasePiece* originPiece = getPiece(origin);
    BasePiece* destPiece = getPiece(dest);
    bool check = (color == White)?whiteInCheck:blackInCheck;
    bool checkResult;

    printf("Current Dest: {%d, %d}\n", dest.x, dest.y);
    originPiece->legalMovesPrint();

    //make would be move
    originPiece->setPosition(dest);
    board[dest.x][dest.y].clearSquare();
    board[dest.x][dest.y].setPiece(originPiece);
    board[origin.x][origin.y].clearSquare();

    //clear check for color of pieces
    if(color == White){
        whiteInCheck = false;
    }
    else{
        blackInCheck = false;
    }

    //populate all legal moves with new move made
    populateAllLegalMoves();
    originPiece->legalMovesPrint();
    //check if you are in check
    checkResult = (color == White)?whiteInCheck:blackInCheck;

    //move pieces back
    originPiece->setPosition(origin);
    board[origin.x][origin.y].clearSquare();
    board[dest.x][dest.y].clearSquare();
    board[origin.x][origin.y].setPiece(originPiece);
    board[dest.x][dest.y].setPiece(destPiece);

    //restore check value for color of pieces
    if(color == White){
        whiteInCheck = check;
    }
    else{
        blackInCheck = check;
    }

    //populate all legal moves with piece moved back
    populateAllLegalMoves();
    originPiece->legalMovesPrint();
    return checkResult;
}

// Definitions for Pawn Class
Pawn::Pawn(Color color, Position pos, Board* board){
    this->color = color;
    this->pos = pos;
    this->board = board;
    doubleJump = true;
    enPassant = true;
    type = PawnType;
}

Pawn::~Pawn(){
#ifdef DEBUG
    printf("Deconstructing Pawn at %d, %d\n", pos.x, pos.y);
#endif
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
    if(board->isInBounds(testPos)){
        piece = board->getPiece(testPos);
        if(piece == NULL){
            legalMoves.push_back(testPos);

            //no piece two spaces in front of pawn & its pawns first move -> it is legal move
            testPos = {pos.x, pos.y + 2*dir};
            if(board->isInBounds(testPos)){
                piece = board->getPiece(testPos);
                #ifdef DEBUG
                    //printf("firstMove as seen by Pawn::populateLegalMoves: %d\n", firstMove);
                #endif
                if(piece == NULL && firstMove){
                    #ifdef DEBUG
                        printf("double jump is legal move\n");
                    #endif
                    legalMoves.push_back(testPos);
                }
            }
        }
    }

    //if there is an enemy piece diagonal to the pawn it is a legal move
    testPos = {pos.x+1, pos.y + 1*dir};
    if(board->isInBounds(testPos)){
        piece = board->getPiece(testPos);
        if(piece != NULL){
            if(piece->getColor() != color){
                //We are attacking a king, set check
                if(piece->getType() == KingType){
                    printf("Setting check\n");
                    board->setCheck((color==White) ? Black : White ,true);
                }
                legalMoves.push_back({pos.x+1, pos.y+1*dir});
            }
        }
    }
    testPos = {pos.x-1, pos.y + 1*dir};
    if(board->isInBounds(testPos)){
        piece = board->getPiece(testPos);
        if(piece != NULL){
            if(piece->getColor() != color){
                //We are attacking a king, set check
                if(piece->getType() == KingType){
                    printf("Setting check\n");
                    board->setCheck((color==White) ? Black : White ,true);
                }
                legalMoves.push_back({pos.x-1, pos.y+1*dir});
            }
        }
    }
    #ifdef DEBUG
        printf("Legal moves for Pawn at {%2d, %2d}:     ", pos.x, pos.y);
        legalMovesPrint();
    #endif
}




// Definitions for the Rook Class
Rook::Rook(Color color, Position pos, Board* board){
    this->color = color;
    this->pos = pos;
    this->board = board;
    type = RookType;
}

Rook::~Rook(){
#ifdef DEBUG
    printf("Deconstructing Rook at %d, %d\n", pos.x, pos.y);
#endif
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
    
    for(int i = 1; i < 8; i++){
        //left
        if(left){
            newPos = {pos.x-i, pos.y};
            #ifdef DEBUG
                printf("Rook @ {%d, %d}: Checking {%d, %d} in left direction\n", pos.x, pos.y, newPos.x, newPos.y);
            #endif
            if(board->isInBounds(newPos)){
                piece = board->getPiece(newPos);
                
                //no piece
                if(piece == NULL){
                    #ifdef DEBUG
                        printf("Move is NULL piece -> adding to move list\n");
                    #endif
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        #ifdef DEBUG
                            printf("Move is enemy piece -> adding to move list -> dir = over\n");
                        #endif
                        //We are attacking a king, set check
                        if(piece->getType() == KingType){
                            printf("Setting check\n");
                            board->setCheck((color==White) ? Black : White ,true);
                        }
                        legalMoves.push_back(newPos);
                        left = false;
                    }
                    else{ //my piece
                        #ifdef DEBUG
                            printf("Move is friend piece -> dir = over\n");
                        #endif
                        left = false;
                    }
                }
            }
            else{ //out of bounds
                #ifdef DEBUG
                    printf("Move is out of bounds -> dir = over\n");
                #endif
                left = false;
            }
        }

        //right
        if(right){
            newPos = {pos.x+i, pos.y};
            #ifdef DEBUG
                printf("Rook @ {%d, %d}: Checking {%d, %d} in right direction\n", pos.x, pos.y, newPos.x, newPos.y);
            #endif
            if(board->isInBounds(newPos)){
                piece = board->getPiece(newPos);
                
                //no piece
                if(piece == NULL){
                    #ifdef DEBUG
                        printf("Move is NULL piece -> adding to move list\n");
                    #endif
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        #ifdef DEBUG
                            printf("Move is enemy piece -> adding to move list -> dir = over\n");
                        #endif
                        //We are attacking a king, set check
                        if(piece->getType() == KingType){
                            printf("Setting check\n");
                            board->setCheck((color==White) ? Black : White ,true);
                        }
                        legalMoves.push_back(newPos);
                        right = false;
                    }
                    else{ //my piece
                        #ifdef DEBUG
                            printf("Move is friend piece -> dir = over\n");
                        #endif
                        right = false;
                    }
                }
            }
            else{ //out of bounds
                #ifdef DEBUG
                    printf("Move is out of bounds -> dir = over\n");
                #endif
                right = false;
            }
        }

        //up
        if(up){
            newPos = {pos.x, pos.y+i};
            #ifdef DEBUG
                printf("Rook @ {%d, %d}: Checking {%d, %d} in up direction\n", pos.x, pos.y, newPos.x, newPos.y);
            #endif
            if(board->isInBounds(newPos)){
                piece = board->getPiece(newPos);
                
                //no piece
                if(piece == NULL){
                    #ifdef DEBUG
                        printf("Move is NULL piece -> adding to move list\n");
                    #endif
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        #ifdef DEBUG
                            printf("Move is enemy piece -> adding to move list -> dir = over\n");
                        #endif
                        //We are attacking a king, set check
                        if(piece->getType() == KingType){
                            printf("Setting check\n");
                            board->setCheck((color==White) ? Black : White ,true);
                        }
                        legalMoves.push_back(newPos);
                        up = false;
                    }
                    else{ //my piece
                        #ifdef DEBUG
                            printf("Move is friend piece -> dir = over\n");
                        #endif
                        up = false;
                    }
                }
            }
            else{ //out of bounds
                #ifdef DEBUG
                    printf("Move is out of bounds -> dir = over\n");
                #endif
                up = false;
            }
        }

        //down
        if(down){
            newPos = {pos.x, pos.y-i};
            #ifdef DEBUG
                printf("Rook @ {%d, %d}: Checking {%d, %d} in down direction\n", pos.x, pos.y, newPos.x, newPos.y);
            #endif
            if(board->isInBounds(newPos)){
                piece = board->getPiece(newPos);
                
                //no piece
                if(piece == NULL){
                    #ifdef DEBUG
                        printf("Move is NULL piece -> adding to move list\n");
                    #endif
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        #ifdef DEBUG
                            printf("Move is enemy piece -> adding to move list -> dir = over\n");
                        #endif
                        //We are attacking a king, set check
                        if(piece->getType() == KingType){
                            printf("Setting check\n");
                            board->setCheck((color==White) ? Black : White ,true);
                        }
                        legalMoves.push_back(newPos);
                        down = false;
                    }
                    else{ //my piece
                        #ifdef DEBUG
                            printf("Move is friend piece -> dir = over\n");
                        #endif
                        down = false;
                    }
                }
            }
            else{ //out of bounds
                #ifdef DEBUG
                    printf("Move is out of bounds -> dir = over\n");
                #endif
                down = false;
            }
        }
    }
    #ifdef DEBUG
        printf("Legal moves for Rook at {%2d, %2d}:     ", pos.x, pos.y);
        legalMovesPrint();
    #endif
}



// Definitions for the Bishop Class
Bishop::Bishop(Color color, Position pos, Board* board){
    this->color = color;
    this->pos = pos;
    this->board = board;
    type = BishopType;
}

Bishop::~Bishop(){
#ifdef DEBUG
    printf("Deconstructing Bishop at %d, %d\n", pos.x, pos.y);
#endif
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

    for(int i = 1; i < 8; i++){
        //up-left
        if(ul){
            newPos = {pos.x-i, pos.y+i};
            #ifdef DEBUG
                printf("Bishop @ {%d, %d}: Checking {%d, %d} in upper right direction\n", pos.x, pos.y, newPos.x, newPos.y);
            #endif
            if(board->isInBounds(newPos)){
                piece = board->getPiece(newPos);
                //no piece
                if(piece == NULL){
                    #ifdef DEBUG
                        printf("Move is NULL piece -> adding to move list\n");
                    #endif
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        #ifdef DEBUG
                            printf("Move is enemy piece -> adding to move list -> dir = over\n");
                        #endif
                        //We are attacking a king, set check
                        if(piece->getType() == KingType){
                            printf("Setting check\n");
                            board->setCheck((color==White) ? Black : White ,true);
                        }
                        legalMoves.push_back(newPos);
                        ul = false;
                    }
                    else{ //my piece
                        #ifdef DEBUG
                            printf("Move is friend piece -> dir = over\n");
                        #endif
                        ul = false;
                    }
                }
            }
            else{ //out of bounds
                #ifdef DEBUG
                    printf("Move is out of bounds -> dir = over\n");
                #endif
                ul = false;
            }
        }

        //up-right
        if(ur){
            newPos = {pos.x+i, pos.y+i};
            #ifdef DEBUG
                printf("Bishop @ {%d, %d}: Checking {%d, %d} in upper right direction\n", pos.x, pos.y, newPos.x, newPos.y);
            #endif
            if(board->isInBounds(newPos)){
                piece = board->getPiece(newPos);
                
                //no piece
                if(piece == NULL){
                    #ifdef DEBUG
                        printf("Move is NULL piece -> adding to move list\n");
                    #endif
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        #ifdef DEBUG
                            printf("Move is enemy piece -> adding to move list -> dir = over\n");
                        #endif
                        if(piece->getType() == KingType){
                            printf("Setting check\n");
                            board->setCheck((color==White) ? Black : White ,true);
                        }
                        legalMoves.push_back(newPos);
                        ur = false;
                    }
                    else{ //my piece
                        #ifdef DEBUG
                            printf("Move is friend piece -> dir = over\n");
                        #endif
                        ur = false;
                    }
                }
            }
            else{ //out of bounds
                #ifdef DEBUG
                    printf("Move is out of bounds -> dir = over\n");
                #endif
                ur = false;
            }
        }

        //down-left
        if(dl){
            newPos = {pos.x-i, pos.y-i};
            #ifdef DEBUG
                printf("Bishop @ {%d, %d}: Checking {%d, %d} in upper right direction\n", pos.x, pos.y, newPos.x, newPos.y);
            #endif
            if(board->isInBounds(newPos)){
                piece = board->getPiece(newPos);
                
                //no piece
                if(piece == NULL){
                    #ifdef DEBUG
                        printf("Move is NULL piece -> adding to move list\n");
                    #endif
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        #ifdef DEBUG
                            printf("Move is enemy piece -> adding to move list -> dir = over\n");
                        #endif
                        if(piece->getType() == KingType){
                            printf("Setting check\n");
                            board->setCheck((color==White) ? Black : White ,true);
                        }
                        legalMoves.push_back(newPos);
                        dl = false;
                    }
                    else{ //my piece
                        #ifdef DEBUG
                            printf("Move is friend piece -> dir = over\n");
                        #endif
                        dl = false;
                    }
                }
            }
            else{ //out of bounds
                #ifdef DEBUG
                    printf("Move is out of bounds -> dir = over\n");
                #endif
                dl = false;
            }
        }

        //down-right
        if(dr){
            newPos = {pos.x+i, pos.y-i};
            #ifdef DEBUG
                printf("Bishop @ {%d, %d}: Checking {%d, %d} in upper right direction\n", pos.x, pos.y, newPos.x, newPos.y);
            #endif
            if(board->isInBounds(newPos)){
                piece = board->getPiece(newPos);
                
                //no piece
                if(piece == NULL){
                    #ifdef DEBUG
                        printf("Move is NULL piece -> adding to move list\n");
                    #endif
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        #ifdef DEBUG
                            printf("Move is enemy piece -> adding to move list -> dir = over\n");
                        #endif
                        if(piece->getType() == KingType){
                            printf("Setting check\n");
                            board->setCheck((color==White) ? Black : White ,true);
                        }
                        legalMoves.push_back(newPos);
                        dr = false;
                    }
                    else{ //my piece
                        #ifdef DEBUG
                            printf("Move is friend piece -> dir = over\n");
                        #endif
                        dr = false;
                    }
                }
            }
            else{ //out of bounds
                #ifdef DEBUG
                    printf("Move is out of bounds -> dir = over\n");
                #endif
                dr = false;
            }
        }
    }
    #ifdef DEBUG
        printf("Legal moves for Bishop at {%2d, %2d}:     ", pos.x, pos.y);
        legalMovesPrint();
    #endif
}




// Definitions for the Knight Class
Knight::Knight(Color color, Position pos, Board* board){
    this->color = color;
    this->pos = pos;
    this->board = board;
    type = KnightType;
}

Knight::~Knight(){
#ifdef DEBUG
    printf("Deconstructing Knight at %d, %d\n", pos.x, pos.y);
#endif
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
    if(board->isInBounds(newPos)){
        piece = board->getPiece(newPos);
        
        //no piece
        if(piece == NULL){
            legalMoves.push_back(newPos);
        }
        else{
            //piece
            if(piece->getColor() != color){ //enemy piece
                if(piece->getType() == KingType){
                    printf("Setting check\n");
                    board->setCheck((color==White) ? Black : White ,true);
                }
                legalMoves.push_back(newPos);
            }
        }
    }

    //up-right
    newPos = {pos.x+1, pos.y+2};
    if(board->isInBounds(newPos)){
        piece = board->getPiece(newPos);
        
        //no piece
        if(piece == NULL){
            legalMoves.push_back(newPos);
        }
        else{
            //piece
            if(piece->getColor() != color){ //enemy piece
                if(piece->getType() == KingType){
                    printf("Setting check\n");
                    board->setCheck((color==White) ? Black : White ,true);
                }
                legalMoves.push_back(newPos);
            }
        }
    }

    //down-left
    newPos = {pos.x-1, pos.y-2};
    if(board->isInBounds(newPos)){
        piece = board->getPiece(newPos);
        
        //no piece
        if(piece == NULL){
            legalMoves.push_back(newPos);
        }
        else{
            //piece
            if(piece->getColor() != color){ //enemy piece
                if(piece->getType() == KingType){
                    printf("Setting check\n");
                    board->setCheck((color==White) ? Black : White ,true);
                }
                legalMoves.push_back(newPos);
            }
        }
    }

    //down-right
    newPos = {pos.x+1, pos.y-2};
    if(board->isInBounds(newPos)){
        piece = board->getPiece(newPos);
        
        //no piece
        if(piece == NULL){
            legalMoves.push_back(newPos);
        }
        else{
            //piece
            if(piece->getColor() != color){ //enemy piece
                if(piece->getType() == KingType){
                    printf("Setting check\n");
                    board->setCheck((color==White) ? Black : White ,true);
                }
                legalMoves.push_back(newPos);
            }
        }
    }

    //left-up
    newPos = {pos.x-2, pos.y+1};
    if(board->isInBounds(newPos)){
        piece = board->getPiece(newPos);
        
        //no piece
        if(piece == NULL){
            legalMoves.push_back(newPos);
        }
        else{
            //piece
            if(piece->getColor() != color){ //enemy piece
                if(piece->getType() == KingType){
                    printf("Setting check\n");
                    board->setCheck((color==White) ? Black : White ,true);
                }
                legalMoves.push_back(newPos);
            }
        }
    }

    //left-down
    newPos = {pos.x-2, pos.y-1};
    if(board->isInBounds(newPos)){
        piece = board->getPiece(newPos);
        
        //no piece
        if(piece == NULL){
            legalMoves.push_back(newPos);
        }
        else{
            //piece
            if(piece->getColor() != color){ //enemy piece
                if(piece->getType() == KingType){
                    printf("Setting check\n");
                    board->setCheck((color==White) ? Black : White ,true);
                }
                legalMoves.push_back(newPos);
            }
        }
    }

    //right-up
    newPos = {pos.x+2, pos.y+1};
    if(board->isInBounds(newPos)){
        piece = board->getPiece(newPos);
        
        //no piece
        if(piece == NULL){
            legalMoves.push_back(newPos);
        }
        else{
            //piece
            if(piece->getColor() != color){ //enemy piece
                if(piece->getType() == KingType){
                    printf("Setting check\n");
                    board->setCheck((color==White) ? Black : White ,true);
                }
                legalMoves.push_back(newPos);
            }
        }
    }

    //right-down
    newPos = {pos.x+2, pos.y-1};
    if(board->isInBounds(newPos)){
        piece = board->getPiece(newPos);
        
        //no piece
        if(piece == NULL){
            legalMoves.push_back(newPos);
        }
        else{
            //piece
            if(piece->getColor() != color){ //enemy piece
                if(piece->getType() == KingType){
                    printf("Setting check\n");
                    board->setCheck((color==White) ? Black : White ,true);
                }
                legalMoves.push_back(newPos);
            }
        }
    }
    #ifdef DEBUG
        printf("Legal moves for Knight at {%2d, %2d}:     ", pos.x, pos.y);
        legalMovesPrint();
    #endif
}




// Definitions for the Queen Class
Queen::Queen(Color color, Position pos, Board* board){
    this->color = color;
    this->pos = pos;
    this->board = board;
    type = QueenType;
}

Queen::~Queen(){
#ifdef DEBUG
    printf("Deconstructing Queen at %d, %d\n", pos.x, pos.y);
#endif
}


/* Function:    Queen::populateLegalMoves
 * Arguments:   None
 * Description: Fills a queen's object with the possible moves it can make
 */
void Queen::populateLegalMoves(){
    legalMoves.clear();

    Position newPos;
    BasePiece* piece;
    bool ul = true, ur = true, dl = true, dr = true;
    bool down = true, up = true, left = true, right = true;

    for(int i = 1; i < 8; i++){
        //up-left
        if(ul){
            newPos = {pos.x-i, pos.y+i};
            #ifdef DEBUG
                printf("Queen @ {%d, %d}: Checking {%d, %d} in upper left direction\n", pos.x, pos.y, newPos.x, newPos.y);
            #endif
            if(board->isInBounds(newPos)){
                piece = board->getPiece(newPos);
                //no piece
                if(piece == NULL){
                    #ifdef DEBUG
                        printf("Move is NULL piece -> adding to move list\n");
                    #endif
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        #ifdef DEBUG
                            printf("Move is enemy piece -> adding to move list -> dir = over\n");
                        #endif
                        if(piece->getType() == KingType){
                            printf("Setting check\n");
                            board->setCheck((color==White) ? Black : White ,true);
                        }
                        legalMoves.push_back(newPos);
                        ul = false;
                    }
                    else{ //my piece
                        #ifdef DEBUG
                            printf("Move is friend piece -> dir = over\n");
                        #endif
                        ul = false;
                    }
                }
            }
            else{ //out of bounds
                #ifdef DEBUG
                    printf("Move is out of bounds -> dir = over\n");
                #endif
                ul = false;
            }
        }

        //up-right
        if(ur){
            newPos = {pos.x+i, pos.y+i};
            #ifdef DEBUG
                printf("Queen @ {%d, %d}: Checking {%d, %d} in upper right direction\n", pos.x, pos.y, newPos.x, newPos.y);
            #endif
            if(board->isInBounds(newPos)){
                piece = board->getPiece(newPos);
                
                //no piece
                if(piece == NULL){
                    #ifdef DEBUG
                        printf("Move is NULL piece -> adding to move list\n");
                    #endif
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        #ifdef DEBUG
                            printf("Move is enemy piece -> adding to move list -> dir = over\n");
                        #endif
                        if(piece->getType() == KingType){
                            printf("Setting check\n");
                            board->setCheck((color==White) ? Black : White ,true);
                        }
                        legalMoves.push_back(newPos);
                        ur = false;
                    }
                    else{ //my piece
                        #ifdef DEBUG
                            printf("Move is friend piece -> dir = over\n");
                        #endif
                        ur = false;
                    }
                }
            }
            else{ //out of bounds
                #ifdef DEBUG
                    printf("Move is out of bounds -> dir = over\n");
                #endif
                ur = false;
            }
        }

        //down-left
        if(dl){
            newPos = {pos.x-i, pos.y-i};
            #ifdef DEBUG
                printf("Queen @ {%d, %d}: Checking {%d, %d} in down left direction\n", pos.x, pos.y, newPos.x, newPos.y);
            #endif
            if(board->isInBounds(newPos)){
                piece = board->getPiece(newPos);
                
                //no piece
                if(piece == NULL){
                    #ifdef DEBUG
                        printf("Move is NULL piece -> adding to move list\n");
                    #endif
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        #ifdef DEBUG
                            printf("Move is enemy piece -> adding to move list -> dir = over\n");
                        #endif
                        if(piece->getType() == KingType){
                            printf("Setting check\n");
                            board->setCheck((color==White) ? Black : White ,true);
                        }
                        legalMoves.push_back(newPos);
                        dl = false;
                    }
                    else{ //my piece
                        #ifdef DEBUG
                            printf("Move is friend piece -> dir = over\n");
                        #endif
                        dl = false;
                    }
                }
            }
            else{ //out of bounds
                #ifdef DEBUG
                    printf("Move is out of bounds -> dir = over\n");
                #endif
                dl = false;
            }
        }

        //down-right
        if(dr){
            newPos = {pos.x+i, pos.y-i};
            #ifdef DEBUG
                printf("Queen @ {%d, %d}: Checking {%d, %d} in down right direction\n", pos.x, pos.y, newPos.x, newPos.y);
            #endif
            if(board->isInBounds(newPos)){
                piece = board->getPiece(newPos);
                
                //no piece
                if(piece == NULL){
                    #ifdef DEBUG
                        printf("Move is NULL piece -> adding to move list\n");
                    #endif
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        #ifdef DEBUG
                            printf("Move is enemy piece -> adding to move list -> dir = over\n");
                        #endif
                        if(piece->getType() == KingType){
                            printf("Setting check\n");
                            board->setCheck((color==White) ? Black : White ,true);
                        }
                        legalMoves.push_back(newPos);
                        dr = false;
                    }
                    else{ //my piece
                        #ifdef DEBUG
                            printf("Move is friend piece -> dir = over\n");
                        #endif
                        dr = false;
                    }
                }
            }
            else{ //out of bounds
                #ifdef DEBUG
                    printf("Move is out of bounds -> dir = over\n");
                #endif
                dr = false;
            }
        }

        //up
        if(up){
            newPos = {pos.x, pos.y+i};
            #ifdef DEBUG
                printf("Queen @ {%d, %d}: Checking {%d, %d} in the up direction\n", pos.x, pos.y, newPos.x, newPos.y);
            #endif
            if(board->isInBounds(newPos)){
                piece = board->getPiece(newPos);
                //no piece
                if(piece == NULL){
                    #ifdef DEBUG
                        printf("Move is NULL piece -> adding to move list\n");
                    #endif
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        #ifdef DEBUG
                            printf("Move is enemy piece -> adding to move list -> dir = over\n");
                        #endif
                        if(piece->getType() == KingType){
                            printf("Setting check\n");
                            board->setCheck((color==White) ? Black : White ,true);
                        }
                        legalMoves.push_back(newPos);
                        up = false;
                    }
                    else{ //my piece
                        #ifdef DEBUG
                            printf("Move is friend piece -> dir = over\n");
                        #endif
                        up = false;
                    }
                }
            }
            else{ //out of bounds
                #ifdef DEBUG
                    printf("Move is out of bounds -> dir = over\n");
                #endif
                up = false;
            }
        }

        //right
        if(right){
            newPos = {pos.x+i, pos.y};
            #ifdef DEBUG
                printf("Queen @ {%d, %d}: Checking {%d, %d} in the right direction\n", pos.x, pos.y, newPos.x, newPos.y);
            #endif
            if(board->isInBounds(newPos)){
                piece = board->getPiece(newPos);
                
                //no piece
                if(piece == NULL){
                    #ifdef DEBUG
                        printf("Move is NULL piece -> adding to move list\n");
                    #endif
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        #ifdef DEBUG
                            printf("Move is enemy piece -> adding to move list -> dir = over\n");
                        #endif
                        if(piece->getType() == KingType){
                            printf("Setting check\n");
                            board->setCheck((color==White) ? Black : White ,true);
                        }
                        legalMoves.push_back(newPos);
                        right = false;
                    }
                    else{ //my piece
                        #ifdef DEBUG
                            printf("Move is friend piece -> dir = over\n");
                        #endif
                        right = false;
                    }
                }
            }
            else{ //out of bounds
                #ifdef DEBUG
                    printf("Move is out of bounds -> dir = over\n");
                #endif
                right = false;
            }
        }

        //down
        if(down){
            newPos = {pos.x, pos.y-i};
            #ifdef DEBUG
                printf("Queen @ {%d, %d}: Checking {%d, %d} in the down direction\n", pos.x, pos.y, newPos.x, newPos.y);
            #endif
            if(board->isInBounds(newPos)){
                piece = board->getPiece(newPos);
                
                //no piece
                if(piece == NULL){
                    #ifdef DEBUG
                        printf("Move is NULL piece -> adding to move list\n");
                    #endif
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        #ifdef DEBUG
                            printf("Move is enemy piece -> adding to move list -> dir = over\n");
                        #endif
                        if(piece->getType() == KingType){
                            printf("Setting check\n");
                            board->setCheck((color==White) ? Black : White ,true);
                        }
                        legalMoves.push_back(newPos);
                        down = false;
                    }
                    else{ //my piece
                        #ifdef DEBUG
                            printf("Move is friend piece -> dir = over\n");
                        #endif
                        down = false;
                    }
                }
            }
            else{ //out of bounds
                #ifdef DEBUG
                    printf("Move is out of bounds -> dir = over\n");
                #endif
                down = false;
            }
        }

        //left
        if(left){
            newPos = {pos.x-i, pos.y};
            #ifdef DEBUG
                printf("Queen @ {%d, %d}: Checking {%d, %d} in the left direction\n", pos.x, pos.y, newPos.x, newPos.y);
            #endif
            if(board->isInBounds(newPos)){
                piece = board->getPiece(newPos);
                
                //no piece
                if(piece == NULL){
                    #ifdef DEBUG
                        printf("Move is NULL piece -> adding to move list\n");
                    #endif
                    legalMoves.push_back(newPos);
                }
                else{
                    //piece
                    if(piece->getColor() != color){ //enemy piece
                        #ifdef DEBUG
                            printf("Move is enemy piece -> adding to move list -> dir = over\n");
                        #endif
                        if(piece->getType() == KingType){
                            printf("Setting check\n");
                            board->setCheck((color==White) ? Black : White ,true);
                        }
                        legalMoves.push_back(newPos);
                        left = false;
                    }
                    else{ //my piece
                        #ifdef DEBUG
                            printf("Move is friend piece -> dir = over\n");
                        #endif
                        left = false;
                    }
                }
            }
            else{ //out of bounds
                #ifdef DEBUG
                    printf("Move is out of bounds -> dir = over\n");
                #endif
                left = false;
            }
        }
    }
    #ifdef DEBUG
        printf("Legal moves for Queen at {%2d, %2d}:     ", pos.x, pos.y);
        legalMovesPrint();
    #endif
}



// Definitions for the King Class
King::King(Color color, Position pos, Board* board){
    this->color = color;
    this->pos = pos;
    this->board = board;
    check = false;
    leftCastle = false;
    rightCastle = false;
    type = KingType;
}

King::~King(){
#ifdef DEBUG
    printf("Deconstructing King at %d, %d\n", pos.x, pos.y);
#endif
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
    if(board->isInBounds(newPos)){
        piece = board->getPiece(newPos);
        
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
    if(board->isInBounds(newPos)){
        piece = board->getPiece(newPos);
        
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
    if(board->isInBounds(newPos)){
        piece = board->getPiece(newPos);
        
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
    if(board->isInBounds(newPos)){
        piece = board->getPiece(newPos);
        
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
    if(board->isInBounds(newPos)){
        piece = board->getPiece(newPos);
        
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
    if(board->isInBounds(newPos)){
        piece = board->getPiece(newPos);
        
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
    if(board->isInBounds(newPos)){
        piece = board->getPiece(newPos);
        
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
    if(board->isInBounds(newPos)){
        piece = board->getPiece(newPos);
        
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

    std::vector<Position> legalMovesVector;
    BasePiece* piece2;

    //Castle Right
    if(firstMove == true && !board->isInCheck(color)){
        for(int i = 1; i <= 4; i++){
            newPos = {pos.x+i, pos.y};
            #ifdef DEBUG
                printf("Obtaining piece at {%d, %d}\n", newPos.x, newPos.y);
            #endif
            piece = board->getPiece(newPos);
            if(i <= 3){
                if(piece != NULL){
                    #ifdef DEBUG
                        printf("Piece is not NULL\n");
                        printf("goto 1: stopRight\n");
                    #endif
                    goto stopRight;
                }
                else{
                    #ifdef DEBUG
                        printf("Piece is NULL -> checking if square is being attacked\n");
                    #endif
                    for(int j = 0; j < 8; j++){
                        for(int k = 0; k < 8; k++){
                            piece2 = board->getPiece({j, k});
                            if(piece2 != NULL){
                                if(piece2->getColor() != color && piece2->getType() != KingType){
                                    #ifdef DEBUG
                                        printf("Checking if {%d, %d} is attacking square in question\n", piece2->getPosition().x, piece2->getPosition().y);
                                    #endif
                                    for(Position pos : piece2->legalMoves){
                                        if(pos.x == newPos.x && pos.y == newPos.y){
                                            #ifdef DEBUG
                                                printf("goto 2: stopRight\n");
                                            #endif
                                            goto stopRight;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else if(piece->getType() == RookType && 
                piece->getFirstMove() && piece->getColor() == color){
                #ifdef DEBUG
                    printf("Castling right legal\n");
                #endif
                legalMoves.push_back({pos.x+2, pos.y});
            }
        }
    }
    stopRight:
    
    //Castle Left
    if(firstMove == true && !board->isInCheck(color)){
        for(int i = 1; i <= 3; i++){
            newPos = {pos.x-i, pos.y};
            #ifdef DEBUG
                printf("Obtaining piece at {%d, %d}\n", newPos.x, newPos.y);
            #endif
            piece = board->getPiece(newPos);
            if(i <= 2){
                if(piece != NULL){

                    #ifdef DEBUG
                        printf("Piece is not NULL\n");
                        printf("goto 1: stopLeft\n");
                    #endif
                    goto stopLeft;
                }
                else{
                    #ifdef DEBUG
                        printf("Piece is NULL -> checking if square is being attacked\n");
                    #endif
                    for(int j = 0; j < 8; j++){
                        for(int k = 0; k < 8; k++){
                            piece2 = board->getPiece({j, k});
                            if(piece2 != NULL){
                                if(piece2->getColor() != color && piece2->getType() != KingType){
                                    #ifdef DEBUG
                                        printf("Checking if {%d, %d} is attacking square in question\n", piece2->getPosition().x, piece2->getPosition().y);
                                    #endif
                                    for(Position pos : piece2->legalMoves){
                                        if(pos.x == newPos.x && pos.y == newPos.y){
                                            #ifdef DEBUG
                                                printf("goto 2: stopLeft\n");
                                            #endif
                                            goto stopLeft;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                
            }
            else if(piece->getType() == RookType && 
                piece->getFirstMove() && piece->getColor() == color){
                #ifdef DEBUG
                    printf("Castling left legal\n");
                #endif
                legalMoves.push_back({pos.x-2, pos.y});
            }
        }
    }
    stopLeft:
    ; // need NULL statement after label because it expects primary expression

    #ifdef DEBUG
        printf("Legal moves for King at {%2d, %2d}:     ", pos.x, pos.y);
        legalMovesPrint();
    #endif
}
