#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

// Mux
#define muxA 1
#define muxB 2
#define muxC 3
#define muxOut 4

// Decoder
#define decA0 5
#define decA1 6
#define decA2 7

#define out0 17
#define out1 16
#define out2 15
#define out3 14

#define input  1
#define output 0

#define matrix_size 2 //2x2 matix

int input_pins[]  = { muxOut };
int output_pins[] = { muxA, muxB, muxC, decA0, decA1, decA2, out0, out1, out2, out3 };
int decoder_sel[] = { decA0, decA1, decA2 };
int multiplexor_sel[] = { muxA, muxB, muxC};

long binary_out[64];

const int scan_delay = 1;

void Set   (int);
void delay (int);
void GPIO_Init  (int);
void Scan_Hall  (void);

void LED_Output (void);

//long decimalToBinary(int decimalNum);

void app_main(void) {
    	GPIO_Init(input);
	GPIO_Init(output);
    
    	while(1) {
        	Scan_Hall();
		LED_Output();
    	}
}

void delay(int duration) { vTaskDelay(duration / portTICK_PERIOD_MS); }

void LED_Output(void){	
	gpio_set_level(out0, binary_out[0]);
	gpio_set_level(out1, binary_out[1]);
	gpio_set_level(out2, binary_out[2]);
	gpio_set_level(out3, binary_out[3]);
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
	}
}

void GPIO_Init(int direction) {
    char pin;
    char size  = (direction ? sizeof(input_pins ) : sizeof(output_pins)) / sizeof(int);
    
    for (int i = 0; i < size; i++) {
        pin = direction ? input_pins[i] : output_pins[i];
        
        gpio_reset_pin(pin);
        gpio_set_direction(pin, ( direction ? GPIO_MODE_INPUT : GPIO_MODE_OUTPUT ));
    }
}



//long decimalToBinary(int decimalNum) {
//    long binaryNum = 0;
//    int remainder, temp = 1;
//
//    while (decimalNum != 0) {
//        remainder  %= decimalNum % 2;
//        decimalNum /= decimalNum / 2;
//        binaryNum   = binaryNum + remainder * temp;
//        temp = temp * 10;
//    }
//
//    return binaryNum;
//}

// decimalToBinary function came from:
// https://www.log2base2.com/c-examples/loop/split-a-number-into-digits-in-c.html
