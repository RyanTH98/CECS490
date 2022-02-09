#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#define mux0 2
#define mux1 3
#define mux2 4
#define decoder0 6
#define decoder1 7
#define decoder2 8

void app_main(void)
{
	gpio_reset_pin(mux0);
	gpio_reset_pin(mux1);
	gpio_reset_pin(mux2);
	gpio_reset_pin(decoder0);
	gpio_reset_pin(decoder1);
	gpio_reset_pin(decoder2);

	// Set the GPIO as a push/pull output
	gpio_set_direction(mux0, GPIO_MODE_OUTPUT);
	gpio_set_direction(mux1, GPIO_MODE_OUTPUT);
	gpio_set_direction(mux2, GPIO_MODE_OUTPUT);
	gpio_set_direction(decoder0, GPIO_MODE_OUTPUT);
	gpio_set_direction(decoder1, GPIO_MODE_OUTPUT);
	gpio_set_direction(decoder2, GPIO_MODE_OUTPUT);
   
	 while(1) {
		//(mux0, decoder0): 00 => hal0, 01 => hal2, 10 => hal3, 11=> hal4
		gpio_set_level(mux0, 0);
		gpio_set_level(mux1, 0);
		gpio_set_level(mux2, 0);
		gpio_set_level(decoder0, 0);
		gpio_set_level(decoder1, 0);
		gpio_set_level(decoder2, 1);
        	// Blink off (output low)
        	//printf("Turning off the LED ---- EDIT!!\n");
        	//gpio_set_level(BLINK_GPIO, 0);
        	//vTaskDelay(1000 / portTICK_PERIOD_MS);
        	// Blink on (output high)
        	//printf("Turning on the LED\n");
        	//gpio_set_level(BLINK_GPIO, 1);
        	//vTaskDelay(1000 / portTICK_PERIOD_MS);
    	}	
}
