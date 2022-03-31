#include <stdio.h>
#include <math.h>
#include <typeinfo>
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
    ST_SET_ACTIVITY,
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
    bool isWhiteTurn;
    IOController::Move origin1;
    IOController::Move origin2;
    IOController::Move dest1;
    IOController::Move dest2;
    IOController::Move temp;
} State_Machine_Controller;

//function prototypes
void delay_1ms (int);
void LedStrip_Output(void);

extern "C" void app_main() {
    printf("CODE BEGIN\n");
	
   	//pixels.begin(); 

	//set up initial values for state machine
	State_Machine_Controller smc;
	smc.currentState = ST_GAME_START;
	smc.isWhiteTurn = true;
	smc.isNewGame = true;

	// - Create needed objects
	IOController::HalController hc( GPIO_NUM_5,  GPIO_NUM_18,  GPIO_NUM_19,  GPIO_NUM_2,  GPIO_NUM_4, GPIO_NUM_27,  GPIO_NUM_26,  GPIO_NUM_25,  GPIO_NUM_33);
	Chess::Board board;
	//LedController::LedController led_controller(gpio_num_t GPIO_NUM_23);

	//begin
    while(1) {
		//LedStrip_Output();
		
		switch (smc.currentState) {
		    case ST_GAME_START:
		    {
		        // Set up code to begin a game
		    	#ifdef DEBUG
				    printf("Entering ST_GAME_START\n");
				#endif
		        // - Start IO interfacing
		        hc.start();
		        // - Initialize LED controller
		        
		        smc.currentState = ST_START_POS;
		        break;
		    }
		    case ST_START_POS:
		    {
		        // Are all pieces where they should be at the start of a game?
		    	#ifdef DEBUG
				    printf("Entering ST_START_POS\n");
				#endif

		    	if(hc.checkStartingPosition()){
		    		// True  -  GOTO ST_SET_COLOR
		    		smc.currentState = ST_SET_COLOR;
		    	}
		        // False - Loop
		        break;
		    }
		    case ST_SET_COLOR:
		    {
		        // Match the active color to the player whose turn it is
				#ifdef DEBUG
				    printf("Entering ST_SET_COLOR\n");
				#endif
		    	// If first move of the game
		    	if(smc.isNewGame){
		    		// True - Whites Turn
		    		smc.isNewGame = false;
		    	}
		    	else{
		    		// False - swap turns
	    			smc.turnColor  = (smc.turnColor == Chess::White)?Chess::Black:Chess::White;
		    	}

		    	// GOTO ST_WAIT_FOR_LIFT
		        smc.currentState = ST_WAIT_FOR_LIFT;
		        break;
		    }
		    case ST_WAIT_FOR_LIFT:
		    {
		        // Has a piece been lifted off the board?
		    	#ifdef DEBUG
				    printf("Entering ST_WAIT_FOR_LIFT\n");
				#endif
		    	//Wait for new change on board
		    	smc.origin1 = hc.detectChange();						//should i check if the ppiece is being lifted or places? possible edge case

		        // GOTO ST_CHECK_COLOR
		        smc.currentState = ST_CHECK_COLOR;
		        break;
		    }
		    case ST_CHECK_COLOR:
		    {
		        // It's a player's turn. Is the lifted piece theirs?
		    	#ifdef DEBUG
				    printf("Entering ST_CHECK_COLOR\n");
				#endif

		    	Chess::BasePiece* piece = board.getPiece({smc.origin1.targetSquare.x, smc.origin1.targetSquare.y});
		    	if(piece->getColor() == smc.turnColor){
		    		// True  - GOTO ST_SET_ACTIVITY
		    		smc.currentState = ST_SET_ACTIVITY;
		    	}
		        else{
		        	// GOTO ST_RESET_PIECE
		        	smc.currentState = ST_RESET_PIECE;
		        }
		        break;
		    }
		    case ST_SET_ACTIVITY:
		    {
		        // Set a variable to indicate the starting position of the move
		        #ifdef DEBUG
				    printf("Entering ST_SET_ACTIVITY\n");
				#endif

		        //Wait for new change on board
		        smc.temp = hc.detectChange();
		        
		        // Has another piece been picked up?
		    	if(smc.temp.risingEdge){
		    		// Activity is a second lifted piece -> set its origin
		    		smc.origin2 = smc.temp;
		    		// GOTO ST_CHECK_SECOND_COLOR
		    		smc.currentState = ST_CHECK_SECOND_COLOR;
		    	}
				// Has the piece been placed back down?
		        else{
		        	//Activity is a piece placement -> set its dest

		        	// Is piece placement a King initiating castling?
		        		//psudocode
		    		/*	Chess::BasePiece* piece1 = board.getPiece({smc.origin1.targetSquare.x, smc.origin1.targetSquare.y});
		    		 *	if(piece1.checkCastle()){
		    		 *		// GOTO Castle
		    		 *	}
		    		 */

		        	smc.dest1 = smc.temp;

		        	// GOTO ST_VALID_CHECK
		        	smc.currentState = ST_VALID_CHECK;
		        }
		        break;
		    }
		    case ST_CHECK_SECOND_COLOR:
		    {
		        // Does (first piece color == second piece color)?
		    	#ifdef DEBUG
				    printf("Entering ST_CHECK_SECOND_COLOR\n");
				#endif

		        Chess::BasePiece* piece1 = board.getPiece({smc.origin1.targetSquare.x, smc.origin1.targetSquare.y});
		        Chess::BasePiece* piece2 = board.getPiece({smc.origin2.targetSquare.x, smc.origin2.targetSquare.y});
		    	if(piece1->getColor() != piece2->getColor()){
		    		// GOTO ST_CHECK_TAKE
		            smc.currentState = ST_CHECK_TAKE;
		    	}
		        else{
		            // GOTO ST_RESET_PIECE
		        	smc.currentState = ST_RESET_PIECE;
		        }
		        break;
		    }
		    case ST_CHECK_CASTLE:
		    {
		        // Check if the two pieces lifted up were King/Rook (only if they were in the original starting positions)
		        #ifdef DEBUG
				    printf("Entering ST_CHECK_CASTLE\n");
				#endif
				/*
		    	if(((typeid(origin1) == typeid(Chess::Rook)) && (typeid(origin2) == typeid(Chess::King))) ||
		    	   ((typeid(origin2) == typeid(Chess::Rook)) && (typeid(origin1) == typeid(Chess::King)))) {
		    		// Loop until a piece is placed down
		    		temp = hc.detectChange();
			        // Was the piece placed in the same spot?
			            // True  - ST_SET_ACTIVITY
			            // False - continue

		    	}
		    	else{
		    		// !!!TODO!!! Wait till both pieces are put back where they should be
		    	}
		        
		        

		        
		        // Were both pieces placed in their appropriate squares for castling?
		            // ST_VALID_CHECK
		        */

				// GOTO ST_RESET_PIECE
		        smc.currentState = ST_RESET_PIECE;
		        break;
		    }
		    case ST_CHECK_TAKE:
		    {
		        // Called when the two lifted pieces are different colors
		    	#ifdef DEBUG
				    printf("Entering ST_CHECK_TAKE\n");
				#endif
		        // Wait for piece placement
		        smc.dest1 = hc.detectChange();

		        // Check its dest is the origin of the piece it took
		        if(smc.dest1.targetSquare.x != smc.origin2.targetSquare.x || smc.dest1.targetSquare.y != smc.origin2.targetSquare.y){
		        	// GOTO ST_RESET_PIECE
		        	smc.currentState = ST_RESET_PIECE;
		        }

		        // GOTO ST_VALID_CHECK
		        smc.currentState = ST_VALID_CHECK;
		        // ST_VALID_CHECK
		        break;
		    }
		    case ST_VALID_CHECK:
		    {
		    	#ifdef DEBUG
				    printf("Entering ST_VALID_CHECK\n");
				#endif

		        // Was the piece placed down in the same spot?    
		        if(smc.dest1.targetSquare.x == smc.origin1.targetSquare.x && smc.dest1.targetSquare.y == smc.origin2.targetSquare.y){
		        	// True  - ST_WAIT_FOR_LIFT
		        	smc.currentState = ST_WAIT_FOR_LIFT;
		        }
		        // False - continue
		        
		        // Is the move valid?
		        Chess::BasePiece* piece1 = board.getPiece({smc.origin1.targetSquare.x, smc.origin1.targetSquare.y});
		        if(piece1->validateMove({smc.dest1.targetSquare.x, smc.dest1.targetSquare.y})){
		            // True  - ST_CHECK_GAMEOVER
		            smc.currentState = ST_CHECK_GAMEOVER;
		        }
		        else{
		        	// False - ST_RESET_PIECE
		        	smc.currentState = ST_RESET_PIECE;
		        }
		        
		        break;
		    }
		    case ST_RESET_PIECE:
		    {		        
		    	// Wait until pieces are returned to their starting positions
		    	#ifdef DEBUG
				    printf("Entering ST_RESET_PIECE\n");
				#endif
		        break;
		    }
		    case ST_CHECK_GAMEOVER:
		    {
		        // Check if the opponent is now in checkmate or draw
		        #ifdef DEBUG
				    printf("Entering ST_CHECK_GAMEOVER\n");
				#endif

				/*if(board.getCheckmate || board.getDraw){
					// True  - ST_GAME_END
		            smc.currentState = ST_GAME_END;
				}
				else{
					// False  - ST_SET_COLOR
		            smc.currentState = ST_SET_COLOR;
				}*/
		        break;
		    }
		    case ST_GAME_END:
		    {
		        // End game
		         #ifdef DEBUG
				    printf("Entering ST_GAME_END\n");
				#endif
		        break;
		    }
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

// BigMommaClass {
//     BigMommaClass(int, int);

// private:
//     ThingOne thingOne;
//     ThingTwo thingTwo;
// };

// BigMommaClass::BigMommaClass(int numba1, int numba2)
//     : thingOne(numba1 + numba2), thingTwo(numba1, numba2) {}



// GOTO ST_CHECK_CASTLE
// Move is a castling move
/*Chess::BasePiece* piece1 = board.getPiece({smc.origin1.targetSquare.x, smc.origin1.targetSquare.y});
Chess::BasePiece* piece2 = board.getPiece({smc.origin2.targetSquare.x, smc.origin2.targetSquare.y});
if((piece1->getType() == Chess::Type::KingType) && 
	abs(smc.dest1.targetSquare.x - smc.origin1.targetSquare.x) == 2){
	//wait for rook origin
	smc.origin2 = hc.detectChange();
	if(piece2->getType() == Chess::Type::RookType){
		// wait for rook dest
		smc.dest2 = hc.detectChange();
		if(abs(smc.dest1.targetSquare.x - smc.origin1.targetSquare.x) == 2){ //!!!Todo!!! check this properly -> its just hacky right now
			// !!!TODO!!! Wait for piece to go back to original space
		}
	}
	else{
		// GOTO ST_RESET_PIECE
		smc.currentState = ST_RESET_PIECE;
	}*/