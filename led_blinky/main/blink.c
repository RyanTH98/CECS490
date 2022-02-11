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

const int scan_delay = 1;

int input_pins[] = { muxOut };

int output_pins[] = {
    muxA, muxB, muxC,
    decA0, decA1, decA2,
    out0, out1, out2, out3
};


void hall_scan(void);
void set_inputs(void);
void set_outputs(void);
//void instatiate_pins(void);
//long decimalToBinary(int decimalNum);

void app_main(void) {
    set_inputs();
    set_outputs();
    
    gpio_set_level(muxA, 0);
    gpio_set_level(muxB, 0);
    gpio_set_level(muxC, 0);

    gpio_set_level(decA0, 0);
    gpio_set_level(decA1, 0);
    gpio_set_level(decA2, 0);

    gpio_set_level(decA0, 1);
    gpio_set_level(muxA,  1);
    
    while(1) {
        hall_scan();
        vTaskDelay(scan_delay / portTICK_PERIOD_MS);
    }
}

void hall_scan(void) {
    gpio_set_level(decA0, 0);
    gpio_set_level(muxA,  0);
    
    vTaskDelay(scan_delay / portTICK_PERIOD_MS);
    gpio_set_level(out3,  !gpio_get_level(muxOut));

    gpio_set_level(decA0, 0);
    gpio_set_level(muxA,  1);

    vTaskDelay(scan_delay / portTICK_PERIOD_MS);
    gpio_set_level(out0,  !gpio_get_level(muxOut));

    gpio_set_level(decA0, 1);
    gpio_set_level(muxA,  0);

    vTaskDelay(scan_delay / portTICK_PERIOD_MS);
    gpio_set_level(out1,  !gpio_get_level(muxOut));
    

    gpio_set_level(decA0, 1);
    gpio_set_level(muxA,  1);
    //trigger(out3);
    vTaskDelay(scan_delay / portTICK_PERIOD_MS);
    gpio_set_level(out2,  !gpio_get_level(muxOut));
}

void set_inputs(void) {
    int pin;
    
    for (int i = 0; i < sizeof(input_pins)/sizeof(input_pins[0]); i++) {
        pin = input_pins[i];
        
        gpio_reset_pin(pin);
        gpio_set_direction(pin, GPIO_MODE_INPUT);
    }
}

void set_outputs(void) {
    int pin;
    
    for (int i = 0; i < sizeof(output_pins)/sizeof(output_pins[0]); i++) {
        pin = output_pins[i];
        
        gpio_reset_pin(pin);
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
