#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include "chess.h"
#include "IOController.h"


//led
gpio_num_t led_strip_D0 = GPIO_NUM_23;

int timer_flag = 0;

typedef enum State_Machine {
    ST_GAME_START,
    ST_START_POS,
    ST_SET_COLOR,
    ST_WAIT_FOR_LIFT,
    ST_CHECK_COLOR,
    ST_CHECK_SECOND_COLOR,
    ST_SET_ORIGIN,
    ST_CHECK_CASTLE,
    ST_CHECK_TAKE,
    ST_VALID_CHECK,
    ST_RESET_PIECE,
    ST_CHECK_GAMEOVER,
    ST_GAME_END
} State;

typedef struct StateMachineController {
	State currentState;
    Chess::Color turnColor;
    bool isNewGame;
    IOController::Move origin1;
} State_Machine_Montroller;

//function prototypes
void delay_1ms (int);
void LedStrip_Output(void);

extern "C" void app_main() {
    printf("CODE BEGIN\n");
	
   	//pixels.begin(); 

	//set up initial values for state machine
	State_Machine_Montroller smc;
	smc.currentState = ST_GAME_START;
	smc.isWhiteTurn = true;
	smc.isNewGame = true;

	//begin
    while(1) {
		//LedStrip_Output();
		
		switch (currentState) {
		    case ST_GAME_START:
		        // Set up code to begin a game
		    	// - Create needed objects
		    	IOController::HalController hal_controller(gpio_num_t GPIO_NUM_5, gpio_num_t GPIO_NUM_18, gpio_num_t GPIO_NUM_19, gpio_num_t GPIO_NUM_2, gpio_num_t GPIO_NUM_4, 
                        gpio_num_t GPIO_NUM_27, gpio_num_t GPIO_NUM_26, gpio_num_t GPIO_NUM_25, gpio_num_t GPIO_NUM_33);
		    	Chess::Board board();

		        // - Start IO interfacing
		        hal_controller.start();
		        // - Initialize LED controller
		        
		        currentState = ST_START_POS;
		        break;
		    case ST_START_POS:
		        // Are all pieces where they should be at the start of a game?
		    	if(hal_controller.checkStartingPosition()){
		    		// True  -  GOTO ST_SET_COLOR
		    		currentState = ST_SET_COLOR;
		    	}
		        // False - Loop
		        break;
		    case ST_SET_COLOR:
		        // Match the active color to the player whose turn it is

		    	//if first move it is whites turn
		    	if(smc.isNewGame){
		    		smc.isNewGame = false;
		    	}
		    	//otherwise swap turns
		    	else{
	    			smc.turnColor  = (smc.turnColor == White)?Black:White;
		    	}

		    	// GOTO ST_WAIT_FOR_LIFT
		        currentState = ST_WAIT_FOR_LIFT;
		        break;
		    case ST_WAIT_FOR_LIFT:
		        // Has a piece been lifted off the board?

		    	origin1 = hal_controller.detectChange()
		        // GOTO ST_CHECK_COLOR
		        currentState = ST_CHECK_COLOR;
		        break;
		    case ST_CHECK_COLOR:
		        // It's a player's turn. Is the lifted piece theirs?

		    	if(board.getPiece(origin1.targetSquare).getColor() == turnColor){
		    		// True  - GOTO ST_SET_ORIGIN
		    		currentState = ST_SET_ORIGIN;
		    	}
		        else{
		        	// !!!TODO!!!
		            // False - Wait for the piece to be put back and GOTO ST_WAIT_FOR_LIFT
		        }
		        break;
		    case ST_SET_ORIGIN:
		        // Set a variable to indicate the starting position of the move
		        
		        // Has the piece been placed back down?
		            // True  - ST_VALID_CHECK
		            // False - continue

		        // Has another piece been picked up?
		            // True  - ST_CHECK_SECOND_COLOR
		            // False - Loop
		        break;
		    case ST_CHECK_SECOND_COLOR:
		        // Does (first piece color == second piece color)?
		            // True  - ST_CHECK_CASTLE
		            // False - ST_CHECK_TAKE
		        break;
		    case ST_CHECK_CASTLE:
		        // Called when the two lifted pieces are the same color
		        
		        // Check if the two pieces lifted up were King/Rook (only if they were in the original starting positions)

		        // Loop until a piece is placed down
		        
		        // Was the piece placed in the same spot?
		            // True  - ST_SET_ORIGIN
		            // False - continue
		        
		        // Were both pieces placed in their appropriate squares for castling?
		            // ST_VALID_CHECK
		        break;
		    case ST_CHECK_TAKE:
		        // Called when the two lifted pieces are different colors
		        // ST_VALID_CHECK
		        break;
		    case ST_VALID_CHECK:
		        // Was the piece placed down in the same spot?
		            // True  - ST_WAIT_FOR_LIFT
		            // False - continue
		        
		        // Is the move valid?
		            // True  - ST_CHECK_GAMEOVER
		            // False - ST_RESET_PIECE
		        break;
		    case ST_RESET_PIECE:
		        // Wait until pieces are returned to their starting positions
		        break;
		    case ST_CHECK_GAMEOVER:
		        // Check if the opponent is now in checkmate
		        break;
		    case ST_GAME_END:
		        // End game
		        break;
		}
	}
}


void delay_1ms(int duration) { vTaskDelay(duration / portTICK_PERIOD_MS); }


/* Function:    LedStripOutput
 * Arguments:   None
 * Description: Turns on/off the board's LEDs based on the values found from Scan_Hall
 */
/*
void LedStrip_Output(void) {
	int row, column, led;
	pixels.clear(); // Set all pixel colors to 'off'

  	for(int i=0; i<MATRIX_SIZE*MATrix_SIZEE; i++) {
		row = i % MATRIX_SIZE;
		column = i / MATRIX_SIZE;
					
    			
		led = (column%2 == 0)?i:((column+1)*MATRIX_SIZE - row - 1);
		

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
*/

