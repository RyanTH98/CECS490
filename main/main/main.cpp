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
    bool isNewGame, enPass;
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
	smc.enPass = true;
	//IOController::RGBColor white_80 = {255, 255, 255}; // Orange
	//IOController::RGBColor white_20 = {5, 5, 5};  // Blue
	//IOController::RGBColor white_80 = {200, 120, 0}; // Orange
	IOController::RGBColor white_80 = {75, 0, 75}; // Orange
	IOController::RGBColor white_20 = {0, 0, 75};  // Blue


	// - Create needed objects
	IOController::HalController hc(GPIO_NUM_5,  GPIO_NUM_18, GPIO_NUM_19,  GPIO_NUM_2,  GPIO_NUM_4, GPIO_NUM_27,  GPIO_NUM_26,  GPIO_NUM_25,  GPIO_NUM_33);
	Chess::Board board;
	IOController::LedController lc(GPIO_NUM_23, white_20, white_80); 

	//begin
    while(1) {
		switch (smc.currentState) {
		    case ST_GAME_START:
		    {
				// Begin Game
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

				// Illuminate white LEDs
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

		    	// If it's the first move of a game
		    	if(smc.isNewGame) {
		    		// True - White's Turn
		    		smc.isNewGame = false;

		    		#ifdef DEBUG
				    	printf("Set newGame to false (current turnColor is: %s)\n", (smc.turnColor == Chess::White) ? "White" : "Black");
					#endif
		    	}
		    	else{
		    		// False - Swap turns
	    			smc.turnColor  = (smc.turnColor == Chess::White) ? Chess::Black:Chess::White;

	    			#ifdef DEBUG
				    	printf("New turnColor is: %s\n", (smc.turnColor == Chess::White)?"White":"Black");
					#endif
		    	}

		        smc.currentState = ST_WAIT_FOR_LIFT;

		        break;
		    }
		    case ST_WAIT_FOR_LIFT:
		    {
				#ifdef DEBUG
				    printf("Entering ST_WAIT_FOR_LIFT\n");
				#endif
				
				lc.defaultLedUpdate();

		        // A chess piece is lifted

		    	// Wait for new change on board
		    	smc.origin1 = hc.detectChange(); 
				
				// NOTE: "should i check if the ppiece is being lifted or places? possible edge case"

		    	#ifdef DEBUG
				    printf("A piece has been lifted\n");
						
					int row = smc.origin1.position.x;
					int column = smc.origin1.position.y;	
				    printf("Setting its origin at column: %d, row: %d\n", row, column);
				#endif

		        smc.currentState = ST_CHECK_COLOR;
		        break;
		    }
		    case ST_CHECK_COLOR:
		    {
		    	#ifdef DEBUG
				    printf("Entering ST_CHECK_COLOR\n");
				#endif

				int row = smc.origin1.position.x;
				int column = smc.origin1.position.y;

				#ifdef DEBUG
				    printf("Getting piece that was at collum: %d, row: %d\n", row, column);
				#endif    

		        // It's a player's turn. Is the lifted piece theirs?

				// "piece" pointer instantiation
				Chess::BasePiece* piece = board.getPiece({row, column});
				
				#ifdef DEBUG
				    printf("Its piece color is: %s\n", (piece->getColor() == Chess::White) ? "White":"Black");
				    printf("Its turnColor is: %s\n",       (smc.turnColor == Chess::White) ? "White":"Black");
				#endif 
		    	if(piece->getColor() == smc.turnColor) {	
		    		// Update the led array with the lifted pieces legal moves
		    		printf("Updating LED Vector\n");

					std::vector<Chess::Position> legalMoves = piece->getLegalMoves();
	
					std::vector<Chess::Position> takingMoves;
					std::vector<Chess::Position> normalMoves;
					Chess::BasePiece* piece2; 

					for(Chess::Position move : legalMoves){
						piece2 = board.getPiece(move);
						//for pawn taking moves, mostly for enPassant impl
						if(piece->getType() == Chess::PawnType && row - move.x != 0){
							takingMoves.push_back(move);
						}
						else if(piece2 != NULL) {
							takingMoves.push_back(move);
						} else {
							normalMoves.push_back(move);
						}
					}

					//lc.vectorLedUpdate(takingMoves, IOController::RGBColor{0,255,0});
					//lc.vectorLedUpdate(normalMoves, IOController::RGBColor{255,255,255});
					lc.vectorLedUpdate(takingMoves, IOController::RGBColor{0,255,0});
					lc.vectorLedUpdate(normalMoves, IOController::RGBColor{255,255,255});

					// The piece has no legal moves
					if(piece->getLegalMoves().size() == 0) {
						smc.currentState = ST_RESET_PIECE;
					}
					else{
		    			smc.currentState = ST_SET_ACTIVITY;
					}
		    	}
		        else{
		        	smc.currentState = ST_RESET_PIECE;
		        }
		        break;
		    }
		    case ST_SET_ACTIVITY:
		    {
		        #ifdef DEBUG
				    printf("Entering ST_SET_ACTIVITY\n");
				#endif
		        // Set a variable to indicate the starting position of the move
				
		        //Wait for new change on board
		        smc.temp = hc.detectChange();
				smc.dest1 = smc.temp;

				int originX = smc.temp.position.x;
				int originY = smc.temp.position.y;
				int destinationX = smc.origin1.position.x;
				int destinationY = smc.origin1.position.y;

		        #ifdef DEBUG
				    printf("SMC change recieved of -> rising edge : %d at {column: %d, row: %d}\n", 
				    	smc.temp.risingEdge, originX, originY);
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



		        	#ifdef DEBUG

					    printf("A piece has been placed\n");
					    printf("Setting its destination at column: %d, row: %d\n", destinationX, destinationY);
					#endif

					// If returned to original square
					if(originX == destinationX && originY == destinationY){
			        	smc.currentState = ST_WAIT_FOR_LIFT;

			        	#ifdef DEBUG
						    printf("Piece was placed back at its origin\n");
						#endif
					}
					else{
						Chess::BasePiece* piece1 = board.getPiece({smc.origin1.position.x, smc.origin1.position.y});
		        		Chess::BasePiece* piece2 = board.getPiece({smc.dest1.position.x, smc.dest1.position.y});
						if(piece1 != NULL){
							if(piece1->getType() == Chess::PawnType){
								if(smc.origin1.position.x - smc.dest1.position.x != 0){
									if(piece2 == NULL){
										smc.enPass = true;
									}
								}
							}
						}
						
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
					    printf("Setting its origin at column: %d, row: %d\n", destinationX, destinationY);
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

				Chess::BasePiece* piece1 = board.getPiece({smc.origin1.position.x, smc.origin1.position.y});
		        Chess::BasePiece* piece2 = board.getPiece({smc.origin2.position.x, smc.origin2.position.y});

		        // Check its dest is the origin of the piece it took
		        if(smc.dest1.position.x != smc.origin2.position.x || smc.dest1.position.y != smc.origin2.position.y){
					//is it because you are doing enpassant?
					if(piece1->getType() == Chess::PawnType && piece2->getType() == Chess::PawnType){
						// GOTO ST_VALID_CHECK
						smc.enPass = true;
		        		smc.currentState = ST_VALID_CHECK;
					}
					else{
						// GOTO ST_RESET_PIECE
		        		smc.currentState = ST_RESET_PIECE;
					}
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
					lc.defaultLedUpdate();
		        	smc.currentState = ST_WAIT_FOR_LIFT;
		        }
		        // False - continue
		        
		        // Is the move valid?
		        #ifdef DEBUG
				    printf("Getting piece that was at collum: %d, row: %d\n", smc.origin1.position.x, smc.origin1.position.y);
				#endif   
		        Chess::PieceType type = board.getPiece({smc.origin1.position.x, smc.origin1.position.y})->getType();
		        #ifdef DEBUG
				    printf("Checking if move to: %d, row: %d is a valid move\n", smc.dest1.position.x, smc.dest1.position.y);
				#endif   
		        if(board.movePiece({smc.origin1.position.x, smc.origin1.position.y}, {smc.dest1.position.x, smc.dest1.position.y})){
		            // True  - ST_CHECK_GAMEOVER
					printf("--- resetting board ---\n");
					lc.defaultLedUpdate();
					printf("Going to check Gameover from validcheck\n");
		            smc.currentState = ST_CHECK_GAMEOVER;
		            #ifdef DEBUG
					    printf("Move is a valid move\n");
					#endif   

					//if move is a castling move, go to reset piece intead to force rook move
					//also change to other players turn
					if(type == Chess::KingType){
						if(abs(smc.origin1.position.x - smc.dest1.position.x) != 1){
							smc.currentState = ST_RESET_PIECE;
							smc.turnColor = (smc.turnColor == Chess::White)? Chess::Black : Chess::White;
						}
					}

					//if the move was an enpassant move go to reset piece instead
					//also change to other players turn
					if(smc.enPass){
						smc.enPass = false;
						smc.currentState = ST_RESET_PIECE;
						smc.turnColor = (smc.turnColor == Chess::White)? Chess::Black : Chess::White;
					}
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
				
				//#define DEBUG_A
				#ifdef 	DEBUG_A	
					IOController::RGBColor red = {255, 0, 0};

					// Vector of "return to" positions
					std::vector<Chess::Position> originVector, destinationVector;
					IOController::Move nextActivity;
					
					// Add first position to originVector
					originVector.push_back(smc.origin1.position);
					lc.vectorLedUpdate(originVector, red);
					
					// Loop here until piece(s) is/are returned
					while (1) {
						nextActivity = hc.detectChange();

						// A piece was picked up
						if (nextActivity.risingEdge == 0) {
							bool found = false;
							
							for (Chess::Position destination : destinationVector) {
								if ((destination.x == nextActivity.position.x) && (destination.x == nextActivity.position.x)) {
									found = true;
									break;
								}
							}

							if (found == true) {
								originVector.push_back(nextActivity.position);
								lc.vectorLedUpdate(originVector, red);
							}
						}
						// A piece was placed down
						else if (nextActivity.risingEdge == 1) {
							int i = 0;
							
							destinationVector.push_back(nextActivity.position);

							// Was it returned to (an) origin?
							for (Chess::Position origin : originVector) {
								if ((nextActivity.position.x == origin.x) && (nextActivity.position.y == origin.y)) {
									// Return *fixed* LED back to original state
									int temp = (nextActivity.position.y * 8) + nextActivity.position.x;
									lc.singleLedUpdate(lc.getDefaultLedVector().at(temp));
									lc.LedStrip_Output();

									lc.vectorLedUpdate(originVector, red);

									originVector.erase(originVector.begin() + i);
									break;
								}

								i++;
							}
						}
					}

				#endif


				std::vector<int> pieceLocations = board.getActiveSquares(); // Returns last legal positions
				std::vector<int> activeSensors  = hc.getHalVector();		// Retruns current piece positions
				std::vector<IOController::LED_Light> defaultLedVector  = lc.getDefaultLedVector();		// Retruns current piece positions

				IOController::RGBColor red = {255, 0, 0};
				
				while(pieceLocations != activeSensors){
					for(int i = 0; i < 64; i++){
						if(activeSensors.at(i) != pieceLocations.at(i)){
							//light up square
							lc.singleLedUpdate({{i%8, i/8}, red});
						}
						else{
							lc.singleLedUpdate(defaultLedVector.at(i));
						}
					}
					lc.LedStrip_Output();
					hc.detectChange();
					activeSensors  = hc.getHalVector();
				}
				
				lc.defaultLedUpdate();
				smc.currentState = ST_WAIT_FOR_LIFT;
		        break;
		    }
		    case ST_CHECK_GAMEOVER:
		    {
		        // Check if the opponent is now in checkmate or draw
		        #ifdef DEBUG
				    printf("Entering ST_CHECK_GAMEOVER\n");
				#endif

				if(board.evaluateGameOver((smc.turnColor == Chess::White) ? Chess::Black : Chess::White)){
					// True  - ST_GAME_END
					#ifdef DEBUG
						printf("evaluateGameOver returned true\n");
					#endif
		            smc.currentState = ST_GAME_END;
				}
				else{
					// False  - ST_SET_COLOR
					#ifdef DEBUG
						printf("evaluateGameOver returned false\n");
					#endif
		            smc.currentState = ST_SET_COLOR;
				}
				//smc.currentState = ST_SET_COLOR;
		        break;
		    }
		    case ST_GAME_END:
		    {
		        // End game
		        #ifdef DEBUG
				    printf("Entering ST_GAME_END\n");
				#endif
				/*
				//std::default_random_engine generator;
				//std::uniform_int_distribution<int> distribution(0,255);
				std::vector<IOController::LED_Light> randomVec;
				int r,g,b;
				for(int i = 0; i < 64; i++){
					r = random(0, 255);
					g = random(0, 255);
					b = random(0, 255);
					randomVec.push_back({{i%8, i/8}, {r, g, b}});
				}

				while(true){
					for(int i = 0; i < 64; i++){
						lc.singleLedUpdate(randomVec.at(i));
						randomVec.at(i).rgb_color.r = (randomVec.at(i).rgb_color.r + 1) % 255;
						randomVec.at(i).rgb_color.g = (randomVec.at(i).rgb_color.g + 1) % 255;
						randomVec.at(i).rgb_color.b = (randomVec.at(i).rgb_color.b + 1) % 255;
					}
					lc.LedStrip_Output();
					delay(100);
				}
				*/
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



// To do:
// -- If a piece is lifted and has no possible moves, make origin square red
