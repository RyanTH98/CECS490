#include <vector>
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
        Poaition pos;
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
    };

    class LedController{
        private:
            gpio_num_t pin;
            Adafruit_NeoPixel pixels(64, led_strip_D0, NEO_GRB + NEO_KHZ800);
            std::vector<LED_Light> ledVector;
        public:
            LedController(gpio_num_t placeholderPin);
            virtual ~LedController();
            void start();
            void singleLedUpdate(Position pos, RGBColor rgb_color);
            void vectorLedUpdate(std::vector<{Position, RGBColor}> updateVector);
    };
}
