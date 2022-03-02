#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

//compiler definitions
#define NUM_PIXELS 4


//function prototypes
void mux_test();
void decoder_test();
void led_test();
void init();
void delay_1ms(int);


//globals
//mux
gpio_num_t muxA = GPIO_NUM_5;
gpio_num_t muxB = GPIO_NUM_18;
gpio_num_t muxC = GPIO_NUM_19;
gpio_num_t muxEn_n = GPIO_NUM_2;
gpio_num_t muxY = GPIO_NUM_4;
unsigned short mux_counter = 0;

//decoder
gpio_num_t decoderA0 = GPIO_NUM_27;
gpio_num_t decoderA1 = GPIO_NUM_26;
gpio_num_t decoderA2 = GPIO_NUM_25;
gpio_num_t decoderEn = GPIO_NUM_33;
unsigned short decoder_counter = 0;

//led
gpio_num_t led_strip = GPIO_NUM_23;
Adafruit_NeoPixel pixels(NUM_PIXELS, led_strip, NEO_GRB + NEO_KHZ800);

//other
gpio_mode_t out = GPIO_MODE_OUTPUT;
gpio_mode_t in = GPIO_MODE_INPUT;


//main
extern "C" void app_main(){
	printf("CODE BEGIN\n");
	init();
	printf("turning on mux\n");
	gpio_set_level(muxEn_n, 0);
    	
	printf("turning on decoder\n");
	gpio_set_level(decoderEn, 1);
	

	while(1) {
		//printf("Runnin\n");
		delay_1ms(1000);
		//mux_test();
		//decoder_test();
		led_test();
    	}
}

void mux_test(){	
	printf("setting muxA: %d\n", ((mux_counter >>0) & 0x01));
	printf("setting muxB: %d\n", ((mux_counter >>1) & 0x01));
	printf("setting muxC: %d\n", ((mux_counter >>2) & 0x01));
	gpio_set_level(muxA, ((mux_counter >>0) & 0x01));
	gpio_set_level(muxB, ((mux_counter >>1) & 0x01));
	gpio_set_level(muxC, ((mux_counter >>2) & 0x01));
	delay_1ms(5);
	printf("recieved %d from mux pin %d\n\n", gpio_get_level(muxY), mux_counter);
	mux_counter++;
	mux_counter = mux_counter%8;
}

void decoder_test(){
	printf("setting decoderA0: %d\n", ((decoder_counter >>0) & 0x01));
	printf("setting decoderA1: %d\n", ((decoder_counter >>1) & 0x01));
	printf("setting decoderA2: %d\n", ((decoder_counter >>2) & 0x01));
	gpio_set_level(decoderA0, ((decoder_counter >>0) & 0x01));
	gpio_set_level(decoderA1, ((decoder_counter >>1) & 0x01));
	gpio_set_level(decoderA2, ((decoder_counter >>2) & 0x01));
	delay_1ms(5);
	decoder_counter++;
	decoder_counter = decoder_counter%8;
}

void led_test(){
	pixels.clear(); // Set all pixel colors to 'off'
	
	//set every other pixel on
  	for(int i=0; i<NUM_PIXELS; i++) {
		printf("Setting Led #%d to %d\n", i, i%2);
		if(i%2 == 0){ //off
    			pixels.setPixelColor(i, pixels.Color(0, 0, 0));
		}
		else{ //on    		
			pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    		}
	}
	pixels.show();   // Send the updated pixel colors to the hardware.
}

void delay_1ms(int duration) { vTaskDelay(duration / portTICK_PERIOD_MS); }

void init(){
        gpio_reset_pin(muxEn_n);
        gpio_reset_pin(muxA);
        gpio_reset_pin(muxB);
        gpio_reset_pin(muxC);
        gpio_reset_pin(muxY);
        gpio_reset_pin(decoderA0);
        gpio_reset_pin(decoderA1);
        gpio_reset_pin(decoderA2);
        gpio_reset_pin(decoderEn);
        gpio_set_direction(muxEn_n, out);
        gpio_set_direction(muxA, out);
        gpio_set_direction(muxB, out);
        gpio_set_direction(muxC, out);
        gpio_set_direction(muxY, in);
        gpio_set_direction(decoderA0, out);
        gpio_set_direction(decoderA1, out);
        gpio_set_direction(decoderA2, out);
        gpio_set_direction(decoderEn, out);
	pixels.begin();
}

