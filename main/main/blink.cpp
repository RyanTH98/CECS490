//just fuckin around and editing files

#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
//#define ESP32

// Mux
gpio_num_t muxA = GPIO_NUM_1;
gpio_num_t muxB = GPIO_NUM_2;
gpio_num_t muxC = GPIO_NUM_3;
gpio_num_t muxOut = GPIO_NUM_4;

// Decoder
gpio_num_t decA0 =  GPIO_NUM_5;
gpio_num_t decA1 =  GPIO_NUM_6;
gpio_num_t decA2 =  GPIO_NUM_7;

gpio_num_t out0 =  GPIO_NUM_17;
gpio_num_t out1 =  GPIO_NUM_16;
gpio_num_t out2 =  GPIO_NUM_15;
gpio_num_t out3 =  GPIO_NUM_14;
gpio_num_t led_strip_D0 =  GPIO_NUM_37;

#define input  1
#define output 0

#define matrix_size 3 //2x2 matix

gpio_num_t input_pins[]  = { muxOut };
gpio_num_t output_pins[] = { muxA, muxB, muxC, decA0, decA1, decA2, out0, out1, out2, out3, led_strip_D0 };

long binary_out[64] = {0};

const int scan_delay = 1;
int timer_flag = 0;

void Set   (int);
void delay_1ms (int);
void GPIO_Init  (int);
void Scan_Hall  (void);
void test_led_strip(void);
void LED_Output (void);

#define NUMPIXELS 9 
Adafruit_NeoPixel pixels(NUMPIXELS, led_strip_D0, NEO_GRB + NEO_KHZ800);


extern "C" void app_main(){
	printf("CODE BEGIN\n");
	
	GPIO_Init(input);
	GPIO_Init(output);
   	pixels.begin(); 
    	while(1) {
		//delay(1000);
        	Scan_Hall();
		LED_Output();
		test_led_strip();
    	}
}

void delay_1ms(int duration) { vTaskDelay(duration / portTICK_PERIOD_MS); }

void test_led_strip(){
	int row, column, led;
	pixels.clear(); // Set all pixel colors to 'off'

  	for(int i=0; i<matrix_size*matrix_size; i++) {
		row = i % matrix_size;
		column = i / matrix_size;
					
    			
		led = (column%2 == 0)?i:((column+1)*matrix_size - row - 1);
		

		// pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
  	  	if(binary_out[i] != 0){
			printf("Setting Led #%d High\n", led);
    			pixels.setPixelColor(led, pixels.Color(0, 0, 150));
    			//pixels.setPixelColor(i, pixels.Color(0, 0, 150));
		}
		else{
			printf("Setting LED #%d Low\n", led);
    			pixels.setPixelColor(led, pixels.Color(150, 0, 0));
    			//pixels.setPixelColor(i, pixels.Color(150, 0, 0));
		}
  	}
	pixels.show();   // Send the updated pixel colors to the hardware.
}

void LED_Output(void){	
	gpio_set_level(out0, binary_out[0]);
	gpio_set_level(out1, binary_out[1]);
	gpio_set_level(out2, binary_out[2]);
	gpio_set_level(out3, binary_out[3]);
	for(int i = 0; i<(sizeof(binary_out)/sizeof(binary_out[0])); i++){
		printf("%lu, ", binary_out[i]);
	}
	printf("\n\n");
}

void Scan_Hall(void) {
	unsigned short column;
	unsigned short row;
	for(unsigned short i = 0; i < matrix_size * matrix_size; i++){
		row = i % matrix_size;
		column = i / matrix_size;

   		gpio_set_level(muxA, row & 0x01);
   		gpio_set_level(muxB, (row>>1)&0x01);
   		gpio_set_level(muxC, (row>>2)&0x01);
   		gpio_set_level(decA0, column&0x01);
   		gpio_set_level(decA1, (column>>1)&0x01);
   		gpio_set_level(decA2, (column>>2)&0x01);
    		
		delay(1);
    		binary_out[i] =  !gpio_get_level(muxOut);
		delay(10);
	}
}

void GPIO_Init(int direction) {
    gpio_num_t pin;
    char size  = (direction ? sizeof(input_pins ) : sizeof(output_pins)) / sizeof(int);
    
    for (int i = 0; i < size; i++) {
        pin = direction ? input_pins[i] : output_pins[i];
        
        gpio_reset_pin(pin);
        gpio_set_direction(pin, ( direction ? GPIO_MODE_INPUT : GPIO_MODE_OUTPUT ));
    }
}


