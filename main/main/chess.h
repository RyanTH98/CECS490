#include <vector>
namespace Chess
{
    enum Color{
        White, Black
    };

    typedef struct Coordinates {
        int x;
        int y;
    } Position;

    class BasePiece{
        protected:
            Color color;
            Position pos;
            std::vector<Position> legalMoves;

        public:
            BasePiece();
            virtual ~BasePiece();
            
            Position getPosition();
            void setPosition(Position pos);
            Color getColor();
            std::vector<Position> getLegalMoves();
            bool validateMove(Position dest);
            virtual void populateLegalMoves() = 0;
    };

    class Square{
        private:
            BasePiece* piece;
        
        public:
            Square();
            virtual ~Square();
            
            BasePiece* getPiece();
            void setPiece(BasePiece* piece);
            void clearSquare();
    };

    class Board {
        private:
            Square board[8][8];
            void initBoard();
        
        public:
            Board();
            virtual ~Board();
            
            bool isInBounds(Position pos);
            bool movePiece(Position origin, Position dest);
            BasePiece* getPiece(Position piecePos);
    };

    class Pawn : public BasePiece {
        private:
            bool doubleJump, enPassant;
            
        public:
            Pawn(Color color, Position pos);
            virtual ~Pawn();

            void populateLegalMoves();
    };

    class Rook : public BasePiece {
        private:
        
        public:
            Rook(Color color, Position pos);
            virtual ~Rook();

            void populateLegalMoves();
    };

    class Bishop : public BasePiece {
        private:
        
        public:
            Bishop(Color color, Position pos);
            virtual ~Bishop();

            void populateLegalMoves();
    };

    class Knight : public BasePiece {
        private:
        
        public:
            Knight(Color color, Position pos);
            virtual ~Knight();

            void populateLegalMoves();
    };

    class Queen : public BasePiece {
        private:
        
        public:
            Queen(Color color, Position pos);
            virtual ~Queen();

            void populateLegalMoves();
    };

    class King : public BasePiece {
        private:
            bool check, leftCastle, rightCastle;
        public:
            King(Color color, Position pos);
            virtual ~King();

            void populateLegalMoves();
    };

}