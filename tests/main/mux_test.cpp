#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
//#include "Arduino.h"
//#include <Adafruit_NeoPixel.h>
//#define ESP32


//function prototypes
void mux_test();
void delay_1ms(int);

//globals
gpio_num_t muxA = GPIO_NUM_5;
gpio_num_t muxB = GPIO_NUM_18;
gpio_num_t muxC = GPIO_NUM_19;
gpio_num_t muxEn_n = GPIO_NUM_2;
gpio_num_t muxY = GPIO_NUM_4;

unsigned short mux_counter = 0;
gpio_mode_t out = GPIO_MODE_OUTPUT;
gpio_mode_t in = GPIO_MODE_INPUT;


extern "C" void app_main(){
	printf("CODE BEGIN\n");
	
        gpio_reset_pin(muxEn_n);
        gpio_reset_pin(muxA);
        gpio_reset_pin(muxB);
        gpio_reset_pin(muxC);
        gpio_reset_pin(muxY);
        gpio_set_direction(muxEn_n, out);
        gpio_set_direction(muxA, out);
        gpio_set_direction(muxB, out);
        gpio_set_direction(muxC, out);
        gpio_set_direction(muxY, in);
	printf("turning on mux\n");
	gpio_set_level(muxEn_n, 0);
    	while(1) {
		//printf("Runnin\n");
		delay_1ms(1000);
		mux_test(); 
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
	printf("recieved %d from mux pin %d\n\n", muxY, mux_counter);
	mux_counter++;
	mux_counter = mux_counter%8;
}

void delay_1ms(int duration) { vTaskDelay(duration / portTICK_PERIOD_MS); }


