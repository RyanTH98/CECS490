#ifndef IOCONTROLLER_H
#define IOCONTROLLER_H

#include <vector>
//#include <copy>
#include <stdio.h>
#include <Adafruit_NeoPixel.h>
#include "Arduino.h"

#define DEBOUNCE_COUNT 5

namespace IOController
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

    typedef struct LEDUpdate {
        Position pos;
        RGBColor rgb_color;
    } LED_Light;



    typedef struct Move_Struct{
   		bool risingEdge;
    	Position targetSquare;
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
    };

    class LedController{
        private:
            //gpio_num_t led_strip_D0;
            Adafruit_NeoPixel* pixels;
            std::vector<LED_Light> ledVector;
            RGBColor defaultBlack, defaultWhite;
            void setDefaultLights();
            void start();
        public:
            LedController(gpio_num_t led_strip_D0, RGBColor defaultWhite, RGBColor defaultBlack);
            virtual ~LedController();
            void singleLedUpdate(LED_Light newLED);
            void vectorLedUpdate(std::vector<LED_Light> updateVector);
            void LedStrip_Output();

    };
}
#endif