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

int input_pins[]  = { muxOut };
int output_pins[] = { muxA, muxB, muxC, decA0, decA1, decA2, out0, out1,   out2, out3 };

long binary_out[];

const int scan_delay = 1;

void set   (int);
void delay (int);
void init  (void);
void scan  (void);

//long decimalToBinary(int decimalNum);

void app_main(void) {
    init();
    
    while(1) {
        scan();
    }
}

void delay(int duration) { vTaskDelay(duration / portTICK_PERIOD_MS); }

void init(void) { set(input); set(output); }

void scan(void) {
    int index = 0;
    
    gpio_set_level(decA0, 0);
    gpio_set_level(muxA,  0);
    delay(1);
    binary_out[index] =  !gpio_get_level(muxOut);
    gpio_set_level(out3, !gpio_get_level(muxOut));
    index++;
    
    gpio_set_level(muxA,  1);
    delay(1);
    binary_out[index] =  !gpio_get_level(muxOut);
    gpio_set_level(out0, !gpio_get_level(muxOut));
    index++;
    
    
    gpio_set_level(decA0, 1);
    gpio_set_level(muxA,  0);
    delay(1);
    binary_out[index] =  !gpio_get_level(muxOut);
    gpio_set_level(out1, !gpio_get_level(muxOut));
    index++;
                
    gpio_set_level(muxA,  1);
    delay(1);
    binary_out[index] =  !gpio_get_level(muxOut);
    gpio_set_level(out2, !gpio_get_level(muxOut));
}

void set(int direction) {
    int pin;
    int size  = 0;
    int state = 0;
    int input_size  = sizeof(input_pins)  / sizeof(input_pins[0]);
    int output_size = sizeof(output_pins) / sizeof(output_pins[0]);
    
    if (direction == input) {
        state = input;
        size  = input_size;
    } else if (direction == output) {
        state = output;
        size  = output_size;
    };
    
    for (int i = 0; i < size; i++) {
        state == input ? (pin = input_pins[i]) : (pin = output_pins[i]);
        
        gpio_reset_pin(pin);
        
        if (state == input)
            gpio_set_direction(pin, GPIO_MODE_INPUT);
        else if (state == output)
            gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    }
}







//void instatiate_pins(void) {
//
//}




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
