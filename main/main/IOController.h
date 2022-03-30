#include <vector>
#include <stdio.h>
#include <Adafruit_NeoPixel.h>

namespace IODevices
{
    typedef struct Coordinates {
        int x;
        int y;
    } Position;

    typedef struct RGBColor {
        int r;
        int g;
        int b;
    } RGBColor;


    typedef struct Move{
   		bool hasPiece;
    	Position targetSquare;
    }

    // class IOController{
    //     private:
    //         std::vector<> chessSquare;
    //     public:
    //         void clearBoard();
    // }

    class HalController{
        private:
            std::vector<int> halVector;
            void scan();
            bool debounce();
        public:
            HalController();
            virtual ~HalController();
            
            bool checkStartingPosition();
            Move detectChange();
    };

    class LedController{
        private:
            Adafruit_NeoPixel pixels(matrix_size*matrix_size, led_strip_D0, NEO_GRB + NEO_KHZ800);
            std::vector<int> ledVector;
        public:
            HalController();
            virtual ~HalController();
            void start();
            void singleLedUpdate(Position pos, RGBColor rgb_color);
            void vectorLedUpdate(std::vector<{Position, RGBColor}> updateVector);
    };
}