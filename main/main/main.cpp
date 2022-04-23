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

#ifdef DEBUG
#undef DEBUG
#endif

#ifdef DEBUG_A
#undef DEBUG_A
#endif
#define DEBUG

// When using a pointer, do -->

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
    //bool isWhiteTurn;
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
	//smc.isWhiteTurn = true;
	smc.turnColor = Chess::White;
	smc.isNewGame = true;
	IOController::RGBColor white_80 = {204, 0, 0};
	IOController::RGBColor white_20 = {0, 0, 51};


	// - Create needed objects
	IOController::HalController hc(GPIO_NUM_5,  GPIO_NUM_18, GPIO_NUM_19,  GPIO_NUM_2,  GPIO_NUM_4, GPIO_NUM_27,  GPIO_NUM_26,  GPIO_NUM_25,  GPIO_NUM_33);
	Chess::Board board;
	IOController::LedController lc(GPIO_NUM_23, white_80, white_20); 

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
		    		#ifdef DEBUG
				    	printf("Set newGame to false (current turnColor is: %s)\n", (smc.turnColor == Chess::White)?"White":"Black");
					#endif
		    	}
		    	else{
		    		// False - swap turns
	    			smc.turnColor  = (smc.turnColor == Chess::White)?Chess::Black:Chess::White;
	    			#ifdef DEBUG
				    	printf("New turnColor is: %s\n", (smc.turnColor == Chess::White)?"White":"Black");
					#endif
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

		    	#ifdef DEBUG
				    printf("A piece has been lifted\n");
				    printf("Setting its origin at collum: %d, row: %d\n", smc.origin1.position.x, smc.origin1.position.y);
				#endif
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

				    
				#ifdef DEBUG
				    printf("Getting piece that was at collum: %d, row: %d\n", smc.origin1.position.x, smc.origin1.position.y);
				#endif    

		    	Chess::BasePiece* piece = board.getPiece({smc.origin1.position.x, smc.origin1.position.y});
		    	#ifdef DEBUG
				    printf("Its piece color is: %s\n", (piece->getColor() == Chess::White)?"White":"Black");
				    printf("Its turnColor is: %s\n", (smc.turnColor == Chess::White)?"White":"Black");
				#endif 
		    	if(piece->getColor() == smc.turnColor){		    		
		    		//update the led array with the lifted pieces legal moves
		    		printf("Updating LED Vector\n");
					lc.vectorLedUpdate(piece->getLegalMoves(), IOController::RGBColor{0,255,0});
					
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

		        #ifdef DEBUG
				    printf("SMC change recieved of -> rising edge : %d at {column: %d, row: %d}\n", 
				    	smc.temp.risingEdge, smc.temp.position.x, smc.temp.position.y);
				#endif

		        
		        // Has another piece been placed or picked up?
		    	if(smc.temp.risingEdge){
		    		//Activity is a piece placement -> set its dest

		        	// Is piece placement a King initiating castling?
		        		//psudocode
		    		/*	Chess::BasePiece* piece1 = board.getPiece({smc.origin1.position.x, smc.origin1.position.y});
		    		 *	if(piece1.checkCastle()){
		    		 *		// GOTO Castle
		    		 *	}
		    		 */

		        	smc.dest1 = smc.temp;
		        	#ifdef DEBUG
					    printf("A piece has been placed\n");
					    printf("Setting its destination at collum: %d, row: %d\n", smc.dest1.position.x, smc.dest1.position.y);
					#endif

					if(smc.origin1.position.x == smc.dest1.position.x && smc.origin1.position.y == smc.dest1.position.y){
						// GOTO ST_WAIT_FOR_LIFT
			        	smc.currentState = ST_WAIT_FOR_LIFT;
			        	#ifdef DEBUG
						    printf("Piece was placed back at its origin\n");
						#endif
					}
					else{
			        	// GOTO ST_VALID_CHECK
			        	smc.currentState = ST_VALID_CHECK;
			        	#ifdef DEBUG
						    printf("Piece was placed on new square\n");
						#endif
					}
		    	}
		        else{
		        	// Activity is a second lifted piece -> set its origin
		    		smc.origin2 = smc.temp;
		    		#ifdef DEBUG
					    printf("A 2nd piece has been lifted\n");
					    printf("Setting its origin at collum: %d, row: %d\n", smc.origin1.position.x, smc.origin1.position.y);
					#endif
		    		// GOTO ST_CHECK_SECOND_COLOR
		    		smc.currentState = ST_CHECK_SECOND_COLOR;
		        }
		        break;
		    }
		    case ST_CHECK_SECOND_COLOR:
		    {
		        // Does (first piece color == second piece color)?
		    	#ifdef DEBUG
				    printf("Entering ST_CHECK_SECOND_COLOR\n");
				#endif

		        Chess::BasePiece* piece1 = board.getPiece({smc.origin1.position.x, smc.origin1.position.y});
		        Chess::BasePiece* piece2 = board.getPiece({smc.origin2.position.x, smc.origin2.position.y});

		        #ifdef DEBUG
				    printf("Piece1's color is: %s\n", (piece1->getColor() == Chess::White)?"White":"Black");
				    printf("Piece2's color is: %s\n", (piece2->getColor() == Chess::White)?"White":"Black");
				#endif
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
		        if(smc.dest1.position.x != smc.origin2.position.x || smc.dest1.position.y != smc.origin2.position.y){
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
		        if(smc.dest1.position.x == smc.origin1.position.x && smc.dest1.position.y == smc.origin2.position.y){
		        	// True  - ST_WAIT_FOR_LIFT
		        	#ifdef DEBUG
				    	printf("Piece was put back in same spot, returning to ST_WAIT_FOR_LIFT\n");
					#endif
					lc.setDefaultLights();
		        	smc.currentState = ST_WAIT_FOR_LIFT;
		        }
		        // False - continue
		        
		        // Is the move valid?
		        #ifdef DEBUG
				    printf("Getting piece that was at collum: %d, row: %d\n", smc.origin1.position.x, smc.origin1.position.y);
				#endif   
		        //Chess::BasePiece* piece1 = board.getPiece({smc.origin1.position.x, smc.origin1.position.y});
		        #ifdef DEBUG
				    printf("Checking if move to: %d, row: %d is a valid move\n", smc.dest1.position.x, smc.dest1.position.y);
				#endif   
		        if(board.movePiece({smc.origin1.position.x, smc.origin1.position.y}, {smc.dest1.position.x, smc.dest1.position.y})){
		            // True  - ST_CHECK_GAMEOVER
					printf("--- resetting board ---\n");
					lc.setDefaultLights();
		            smc.currentState = ST_CHECK_GAMEOVER;
		            #ifdef DEBUG
					    printf("Move is a valid move\n");
					#endif   
		        }
		        else{
		        	// False - ST_RESET_PIECE
		        	smc.currentState = ST_RESET_PIECE;
		        	#ifdef DEBUG
					    printf("Move is not a valid move\n");
						printf("- Please return all pieces to their previous locations\n");
					#endif 
		        }
		        
		        break;
		    }
		    case ST_RESET_PIECE:
		    {		        
		    	// Wait until pieces are returned to their starting positions
		    	//#ifdef DEBUG
				    printf("Entering ST_RESET_PIECE\n");
				//#endif
				
				#define DEBUG_A
				#ifdef DEBUG_A
					IOController::RGBColor red = {0, 255, 0};
					std::vector<Chess::Position> newVector;

					IOController::Move nextMove;					
					Chess::Position origin_pos = smc.origin1.position;
					
					// BUG: color of board square doesn't reset after exiting state
					
					newVector.push_back(origin_pos);
					
					lc.vectorLedUpdate(newVector, red);
					
					do {
						nextMove = hc.detectChange();
					} while ((nextMove.position.x != origin_pos.x) && (nextMove.position.x != origin_pos.x));

				#endif

				lc.setDefaultLights();
				smc.currentState = ST_WAIT_FOR_LIFT;
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
				smc.currentState = ST_SET_COLOR;
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


// /* Function:    LedStripOutput
//  * Arguments:   None
//  * Description: Turns on/off the board's LEDs based on the values found from Scan_Hall
//  */

// void LedStrip_Output(BasePiece* piece) {
// 	int row, column, led;
//     Position pos;
//     BasePiece currentPiece;
// 	pixels.clear(); // Set all pixel colors to 'off'
    
//     std::vector<Position> legalMoves = piece.getLegalMoves();
    
//   	for(int i = 0; i < 64; i++) {
// 		row = i % 64;
// 		column = i / 64;
					
    			
// 		led = (column%2 == 0) ? i : ((column + 1) * 64 - row - 1);
//         pos = {row, column};
//         currentPiece = getPiece(pos);
        
//         if(legalMoves[i] == 1) {
//             pixels.setPixelColor(led, pixels.Color(0, 0, 150));
//         }
        
//     			pixels.setPixelColor(led, pixels.Color(0, 0, 150));
// 		}
// 		else{
// 		//	printf("Setting LED #%d Low\n", led);
//     			pixels.setPixelColor(led, pixels.Color(150, 0, 0));
// 		}
//   	}
// 	pixels.show();   // Send the updated pixel colors to the hardware.
// }


// BigMommaClass {
//     BigMommaClass(int, int);

// private:
//     ThingOne thingOne;
//     ThingTwo thingTwo;
// };

// BigMommaClass::BigMommaClass(int numba1, int numba2)
//     : thingOne(numba1 + numba2), thingTwo(numba1, numba2) {}



// // GOTO ST_CHECK_CASTLE
// // Move is a castling move
// Chess::BasePiece* piece1 = board.getPiece({smc.origin1.position.x, smc.origin1.position.y});
// Chess::BasePiece* piece2 = board.getPiece({smc.origin2.position.x, smc.origin2.position.y});
// if((piece1->getType() == Chess::Type::KingType) && 
// 	abs(smc.dest1.position.x - smc.origin1.position.x) == 2){
// 	//wait for rook origin
// 	smc.origin2 = hc.detectChange();
// 	if(piece2->getType() == Chess::Type::RookType){
// 		// wait for rook dest
// 		smc.dest2 = hc.detectChange();
// 		if(abs(smc.dest1.position.x - smc.origin1.position.x) == 2){ //!!!Todo!!! check this properly -> its just hacky right now
// 			// !!!TODO!!! Wait for piece to go back to original space
// 		}
// 	}
// 	else{
// 		// GOTO ST_RESET_PIECE
// 		smc.currentState = ST_RESET_PIECE;
// 	}
