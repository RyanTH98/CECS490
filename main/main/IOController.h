#ifndef IOCONTROLLER_H
#define IOCONTROLLER_H

#include <vector>
//#include <copy>
#include <stdio.h>
#include <Adafruit_NeoPixel.h>
#include "Arduino.h"
#include "chess.h"

//importing position struct from chess.h
typedef Chess::Position Position;

#define DEBOUNCE_COUNT 20

#define DEBUG
#ifdef DEBUG
    #define DEBUG_CHECK_STARTING_POSITION
    #define DEBUG_VECTOR_PRINT
    //#define DEBUG_DETECT_CHANGE
    #define DEBUG_DEBOUNCE
#endif

namespace IOController
{
    // typedef struct Coordinates {
    //     int x;
    //     int y;
    // } Position;

    typedef struct RGBColor {
        int r;
        int g;
        int b;
    } RGBColor;

    typedef struct LED_Light {
        Position pos;
        RGBColor rgb_color;
    } LED_Light;

    typedef struct Move_Struct{ //change name to something better, activity? -bgg
   		bool risingEdge;
    	Position position;
    } Move;


    class HalController{
        private:
            std::vector<int> halVector;
            std::vector<int> scan();
            bool debounce(std::vector<int> debounceHal);
            void enableIO();
            //multiplexor
            gpio_num_t muxA;
            gpio_num_t muxB;
            gpio_num_t muxC;
            gpio_num_t muxEn_n;
            gpio_num_t muxY;

            //decoder
            gpio_num_t decoderA0;
            gpio_num_t decoderA1;
            gpio_num_t decoderA2;
            gpio_num_t decoderEn;
        public:
            HalController(gpio_num_t muxA, gpio_num_t muxB, gpio_num_t muxC, gpio_num_t muxEn_n, gpio_num_t muxY, 
                        gpio_num_t decoderA0, gpio_num_t decoderA1, gpio_num_t decoderA2, gpio_num_t decoderEn);
            virtual ~HalController();
            
            bool checkStartingPosition();
            Move detectChange();
            void start();
            void printBoard();
            std::vector<int> getHalVector();
    };

    class LedController{
        private:
            //gpio_num_t led_strip_D0;
            Adafruit_NeoPixel* pixels;
            std::vector<LED_Light> ledVector;
            std::vector<LED_Light> defaultLedVector;
            RGBColor defaultBlack, defaultWhite;
            void setDefaultLedVector();
            void start();            

        public:
            LedController(gpio_num_t led_strip_D0, RGBColor defaultWhite, RGBColor defaultBlack);
            virtual ~LedController();
            void singleLedUpdate(LED_Light newLED);
            void vectorLedUpdate(std::vector<Position> updateVector, RGBColor color);
            void defaultLedUpdate();
            std::vector<LED_Light> getDefaultLedVector();
            void LedStrip_Output();

    };
}
#endif