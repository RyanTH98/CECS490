#ifndef CHESS_H
#define CHESS_H

#include <vector>
#include <stdio.h>
#include <stdlib.h>

//#define DEBUG

namespace Chess
{
    class Board;

    typedef enum PieceColor
    {
        White,
        Black
    } Color;

    typedef struct Coordinates
    {
        int x;
        int y;
    } Position;

    typedef enum PieceType
    {
        KingType,
        QueenType,
        BishopType,
        RookType,
        KnightType,
        PawnType
    } Type;

    class BasePiece
    {
    protected:
        Color color;
        Position pos;
        
        Type type;
        Board *board;
        bool firstMove;

    public:
        BasePiece();
        ~BasePiece();
        //this should not be public, needed it for castling -> FIX LATER!!!
        std::vector<Position> legalMoves;
        Position getPosition();
        void setPosition(Position pos);
        Color getColor();
        std::vector<Position> getLegalMoves();
        bool validateMove(Position dest);
        Type getType();
        bool getFirstMove();
        void setFirstMove(bool firstMove);
        void legalMovesPrint();
        virtual void populateLegalMoves() = 0;
    };

    class Square
    {
    private:
        BasePiece *piece;

    public:
        Square();
        virtual ~Square();

        BasePiece *getPiece();
        void setPiece(BasePiece *piece);
        void clearSquare();
    };

    class Board
    {
    private:
        Square board[8][8];
        void initBoard();
        bool whiteInCheck;
        bool blackInCheck;
        Position whiteKingLocation;
        Position blackKingLocation;

    public:
        Board();
        virtual ~Board();

        std::vector<int> getActiveSquares();
        bool isInCheck(Color);
        bool isInBounds(Position pos);
        bool movePiece(Position origin, Position dest);
        void printBoard();
        void populateAllLegalMoves();
        void setCheck(Color color, bool check);
        bool evaluateGameOver(Color color);
        bool checkForCheck(Color color, Position origin, Position dest);
        BasePiece *getPiece(Position piecePos);
    };

    class Pawn : public BasePiece
    {
    private:
        bool doubleJump, enPassant;

    public:
        Pawn(Color color, Position pos, Board *board);
        virtual ~Pawn();

        void setEnpassant(bool enPas);
        bool getEnpassant();
        void populateLegalMoves();
    };

    class Rook : public BasePiece
    {
    private:
    public:
        Rook(Color color, Position pos, Board *board);
        virtual ~Rook();

        void populateLegalMoves();
    };

    class Bishop : public BasePiece
    {
    private:
    public:
        Bishop(Color color, Position pos, Board *board);
        virtual ~Bishop();

        void populateLegalMoves();
    };

    class Knight : public BasePiece
    {
    private:
    public:
        Knight(Color color, Position pos, Board *board);
        virtual ~Knight();

        void populateLegalMoves();
    };

    class Queen : public BasePiece
    {
    private:
    public:
        Queen(Color color, Position pos, Board *board);
        virtual ~Queen();

        void populateLegalMoves();
    };

    class King : public BasePiece
    {
    private:
        bool check, leftCastle, rightCastle;

    public:
        King(Color color, Position pos, Board *board);
        virtual ~King();

        void populateLegalMoves();
    };
}

#endif