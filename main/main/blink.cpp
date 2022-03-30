#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

#define input  1
#define output 0
#define matrix_size 8 //nxn matix

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
gpio_num_t led_strip_D0 = GPIO_NUM_23;
Adafruit_NeoPixel pixels(matrix_size*matrix_size, led_strip_D0, NEO_GRB + NEO_KHZ800);

//Just to make life easier
gpio_num_t input_pins[]  = { muxY };
gpio_num_t output_pins[] = { muxA, muxB, muxC, muxEn_n, decoderA0, decoderA1, decoderA2, decoderEn, led_strip_D0 };

long binary_out[64] = {0};

const int scan_delay = 1;
int timer_flag = 0;

//function prototypes
void Set   (int);
void delay_1ms (int);
void GPIO_Init  (int);
void Scan_Hall  (void);
void Serial_Output(void);
void LedStrip_Output(void);



extern "C" void app_main(){
	printf("CODE BEGIN\n");
	
	GPIO_Init(input);
	GPIO_Init(output);

	gpio_set_level(decoderEn, 1);
	gpio_set_level(muxEn_n, 0);
	
   	pixels.begin(); 
    	while(1) {
		//delay(1000);
        	Scan_Hall();
		Serial_Output();
		//LedStrip_Output();
    	}
}

void delay_1ms(int duration) { vTaskDelay(duration / portTICK_PERIOD_MS); }

void LedStrip_Output(void) {
	int row, column, led;
	pixels.clear(); // Set all pixel colors to 'off'

  	for(int i=0; i<matrix_size*matrix_size; i++) {
		row = i % matrix_size;
		column = i / matrix_size;
					
    			
		led = (column%2 == 0)?i:((column+1)*matrix_size - row - 1);
		

		// pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
  	  	if(binary_out[i] != 0){
		//	printf("Setting Led #%d High\n", led);
    			pixels.setPixelColor(led, pixels.Color(0, 0, 150));
		}
		else{
		//	printf("Setting LED #%d Low\n", led);
    			pixels.setPixelColor(led, pixels.Color(150, 0, 0));
		}
  	}
	pixels.show();   // Send the updated pixel colors to the hardware.
}

void Serial_Output(void){	
	for(int i = 0; i < (sizeof(binary_out)/sizeof(binary_out[1])); i++){
		printf("%3d: %lu,    ", i, binary_out[i]);
		if((i+1) % 8 == 0){
			printf("\n");
		}
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
   		gpio_set_level(decoderA0, column&0x01);
   		gpio_set_level(decoderA1, (column>>1)&0x01);
   		gpio_set_level(decoderA2, (column>>2)&0x01);
    		
		delay(10);
		//printf("I recieved %d on sensor %d\n", gpio_get_level(muxY), i);
    		binary_out[i] =  !gpio_get_level(muxY);
		delay(15);
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


