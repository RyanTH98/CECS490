#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
//#include "Arduino.h"
//#include <Adafruit_NeoPixel.h>
//#define ESP32
void toggle_pin();
void delay_1ms(int);

gpio_num_t io34 = GPIO_NUM_34;
int toggle = 0;
gpio_mode_t out = GPIO_MODE_OUTPUT;

extern "C" void app_main(){
	printf("CODE BEGIN\n");
	
        gpio_reset_pin(io34);
        gpio_set_direction(io34, out);
    	while(1) {
		printf("Runnin\n");
		delay_1ms(100);
		toggle_pin(); 
    	}
}

void toggle_pin(){
	gpio_set_level(io34, toggle);
	toggle = (toggle==1)?0:1;
}

void delay_1ms(int duration) { vTaskDelay(duration / portTICK_PERIOD_MS); }


