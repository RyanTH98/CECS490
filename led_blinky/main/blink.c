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

#define num_pins 11

const int scan_delay = 1;

int pins[num_pins] = {1, 2, 3, 4, 5, 6, 7, 14, 15, 16, 17};

//int bs0 = 0;
//int bs1 = 0;
//int bs2 = 0;
//int bs3 = 0;

//void hall_select(int n);
void hall_scan(void);
void reset_pins(void);
//void sensor_value(void);
//void trigger(int port);
//long decimalToBinary(int decimalNum);

void app_main(void) {
    reset_pins();
    
    gpio_set_direction(out0, GPIO_MODE_OUTPUT);
    gpio_set_direction(out1, GPIO_MODE_OUTPUT);
    gpio_set_direction(out2, GPIO_MODE_OUTPUT);
    gpio_set_direction(out3, GPIO_MODE_OUTPUT);
    
	// Mux
    gpio_set_direction(muxA, GPIO_MODE_OUTPUT);
	gpio_set_direction(muxB, GPIO_MODE_OUTPUT);
	gpio_set_direction(muxC, GPIO_MODE_OUTPUT);
    gpio_set_level(muxA, 0);
    gpio_set_level(muxB, 0);
    gpio_set_level(muxC, 0);
    
    // Decoder
    gpio_set_direction(decA0, GPIO_MODE_OUTPUT);
    gpio_set_direction(decA1, GPIO_MODE_OUTPUT);
    gpio_set_direction(decA2, GPIO_MODE_OUTPUT);
    gpio_set_level(decA0, 0);
    gpio_set_level(decA1, 0);
    gpio_set_level(decA2, 0);
    
    // Misc
    //gpio_set_direction(testPin, GPIO_MODE_OUTPUT);
    gpio_set_direction(muxOut, GPIO_MODE_INPUT);
    
//    gpio_set_level(decA0, 1);
//    gpio_set_level(muxA,  1);
    
//    int bsVar =0;
    while(1) {
//        bsVar = gpio_get_level(muxOut);
        hall_scan();
        vTaskDelay(scan_delay / portTICK_PERIOD_MS);
//        if(bsVar) {
//            gpio_set_level(out3, 1);
//            bsVar = 0;
//        } else {
//            gpio_set_level(out3, 0);
//        }
//        vTaskDelay(scan_delay / portTICK_PERIOD_MS);
    }
}

//void hall_select(int n) {
//    gpio_set_level(mux0, 0);
//    gpio_set_level(decoder0, 0);
//}

//void trigger(int port) {
//    int var = gpio_get_level(muxOut);
//
//    if(var) {
//        gpio_set_level(port, 1);
//        var = 0;
//    } else {
//        gpio_set_level(port, 0);
//    }
//}

void hall_scan(void) {
    gpio_set_level(decA0, 0);
    gpio_set_level(muxA,  0);
    
    //trigger(out0);
    vTaskDelay(scan_delay / portTICK_PERIOD_MS);
    gpio_set_level(out3,  !gpio_get_level(muxOut));

    gpio_set_level(decA0, 0);
    gpio_set_level(muxA,  1);

    //trigger(out1);
    vTaskDelay(scan_delay / portTICK_PERIOD_MS);
    gpio_set_level(out0,  !gpio_get_level(muxOut));

    gpio_set_level(decA0, 1);
    gpio_set_level(muxA,  0);
//
//    //trigger(out2);
    vTaskDelay(scan_delay / portTICK_PERIOD_MS);
    gpio_set_level(out1,  !gpio_get_level(muxOut));
    

    gpio_set_level(decA0, 1);
    gpio_set_level(muxA,  1);
    //trigger(out3);
    vTaskDelay(scan_delay / portTICK_PERIOD_MS);
    gpio_set_level(out2,  !gpio_get_level(muxOut));
}

//void sensor_value(void) {
//
//}

void reset_pins(void) {
    for (int pin = 0; pin < num_pins; pin++)
        gpio_reset_pin(pins[pin]);
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

// Blink off (output low)
//printf("Turning off the LED ---- EDIT!!\n");
//gpio_set_level(BLINK_GPIO, 0);
//vTaskDelay(1000 / portTICK_PERIOD_MS);
// Blink on (output high)
//printf("Turning on the LED\n");
//gpio_set_level(BLINK_GPIO, 1);
//vTaskDelay(1000 / portTICK_PERIOD_MS);


// decimalToBinary function came from:
// https://www.log2base2.com/c-examples/loop/split-a-number-into-digits-in-c.html
