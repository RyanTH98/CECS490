#include "IOController.h"

using namespace IOController;


#ifdef DEBUG_VECTOR_PRINT
void halVectorPrint(std::vector<int> vec){
	int i = 0;
    printf("Printing Board as seen by halVectorPrint\n");
	for(int sensor : vec){
		if(i%8 == 0 && i != 0){
			printf("\n");
		}

		printf("%2d: %2d, ", i, sensor);
		
		i++;
	}
	printf("\n\n");
}
void ledVectorPrint(std::vector<LED_Light> vec){
	int i = 0;
    printf("Printing Board as seen by ledVectorPrint\n");
	for(LED_Light light : vec){
		if(i%4 == 0 && i != 0){
			printf("\n");
		}

		printf("%2d:{%2d, %2d}:{%2d, %2d, %2d}, ", 
			i, light.pos.x, light.pos.y, light.rgb_color.r, light.rgb_color.g, light.rgb_color.b);
		i++;
	}
	printf("\n\n");
}
#endif



//definitions for HalScanner Class
HalController::HalController(gpio_num_t muxA, gpio_num_t muxB, gpio_num_t muxC, gpio_num_t muxEn_n, gpio_num_t muxY, 
	gpio_num_t decoderA0, gpio_num_t decoderA1, gpio_num_t decoderA2, gpio_num_t decoderEn){

	//multiplexor
    this->muxA = muxA; //GPIO_NUM_5;
    this->muxB = muxB; //GPIO_NUM_18;
    this->muxC = muxC; //GPIO_NUM_19;
    this->muxEn_n = muxEn_n; //GPIO_NUM_2;
    this->muxY = muxY; //GPIO_NUM_4;

    //decoder
    this->decoderA0 = decoderA0; //GPIO_NUM_27;
    this->decoderA1 = decoderA1; //GPIO_NUM_26;
    this->decoderA2 = decoderA2; //GPIO_NUM_25;
    this->decoderEn = decoderEn; //GPIO_NUM_33;
}

HalController::~HalController(){
#ifdef debug
    printf("Deconstructing HalController\n");
#endif
}

std::vector<int> HalController::scan(){
	std::vector<int> newHalVector;

	for(unsigned short row = 0; row < 8; row++){
		gpio_set_level(decoderA0, row&0x01);
   		gpio_set_level(decoderA1, (row>>1)&0x01);
   		gpio_set_level(decoderA2, (row>>2)&0x01);
		//delay(.005);
		for(unsigned short column = 0;  column < 8; column++){
			gpio_set_level(muxA, column & 0x01);
			gpio_set_level(muxB, (column>>1)&0x01);
			gpio_set_level(muxC, (column>>2)&0x01);
			//delay(10);
			delayMicroseconds(50);

			newHalVector.push_back(!gpio_get_level(muxY));
		}
	}
	return newHalVector;
}
void HalController::enableIO(){
    gpio_reset_pin(muxA);
    gpio_set_direction(muxA, GPIO_MODE_OUTPUT);

    gpio_reset_pin(muxB);
    gpio_set_direction(muxB, GPIO_MODE_OUTPUT);
    
    gpio_reset_pin(muxC);
    gpio_set_direction(muxC, GPIO_MODE_OUTPUT);
    
    gpio_reset_pin(muxEn_n);
    gpio_set_direction(muxEn_n, GPIO_MODE_OUTPUT);
    
    gpio_reset_pin(muxY);
    gpio_set_direction(muxY, GPIO_MODE_INPUT);
    
    gpio_reset_pin(decoderA0);
    gpio_set_direction(decoderA0, GPIO_MODE_OUTPUT);
    
    gpio_reset_pin(decoderA1);
    gpio_set_direction(decoderA1, GPIO_MODE_OUTPUT);
    
    gpio_reset_pin(decoderA2);
    gpio_set_direction(decoderA2, GPIO_MODE_OUTPUT);
    
    gpio_reset_pin(decoderEn);
    gpio_set_direction(decoderEn, GPIO_MODE_OUTPUT);
}

void HalController::start(){
	enableIO();
	gpio_set_level(decoderEn, 1);
	gpio_set_level(muxEn_n, 0);
	halVector = scan();
}

std::vector<LED_Light> HalController::checkStartingPosition(){
	std::vector<LED_Light> missingPiece;
	missingPiece.clear();
	std::vector<int> checkVector = {1,1,1,1,1,1,1,1,
									1,1,1,1,1,1,1,1,
									0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,
									1,1,1,1,1,1,1,1,
									1,1,1,1,1,1,1,1,
								};
	halVector = scan();

	for(int i = 0; i < 64; i++){
		if(halVector.at(i) != checkVector.at(i)){
			missingPiece.push_back({{i%8, i/8}, {255,0,0}});
		}
	}

	#ifdef DEBUG_CHECK_STARTING_POSITION
			printf("Calling printBoard from HalController::checkStartingPosition\n");
			//printBoard();
			printf("halVector:\n");
			halVectorPrint(halVector);
			printf("checkVector:\n");
			halVectorPrint(checkVector);
			printf("Equality: %d\n\n\n", checkVector == halVector);
	#endif
	return missingPiece;
}

bool HalController::debounce(std::vector<int> debounceHal){
	#ifdef DEBUG_DEBOUNCE
		printf("Starting Debounce\n");
	#endif
	for(int i = 0; i < DEBOUNCE_COUNT; i++){
		if(debounceHal != scan()){
			#ifdef DEBUG_DEBOUNCE
				printf("Debounce Failed.\n");
			#endif
			return false;
		}
	}
	#ifdef DEBUG_DEBOUNCE
		printf("Debounce Success.\n");
	#endif
	return true;
}

Move HalController::detectChange(){
	Move move;
	std::vector<int> newHalVector, subtractVector, diffVector;
	bool db_flag = false;
	int i = 0;

	while(!db_flag){
		//aquaire new data from sensors
		newHalVector = scan();
		#ifdef DEBUG_DETECT_CHANGE
			printf("Calling printBoard from HalController::detectChange\n");
			//printBoard();
			printf("halVector:\n");
			vectorPrint(halVector);
			printf("newHalVector:\n");
			vectorPrint(newHalVector);
			printf("Equality: %d\n\n\n", newHalVector == halVector);
		#endif
		//if there is a change in the sensors
		if(halVector != newHalVector){
			//make sure only one change has been made
			//std::set_difference(halVector.begin(), halVector.end(), newHalVector.begin(), newHalVector.end(), 
			//					std::inserter(diffVector, diffVector.begin()));
			for(int i = 0; i < halVector.size(); i++){
				if(halVector[i] != newHalVector[i]){
					diffVector.push_back(1);
				}
			}
			#ifdef DEBUG_DETECT_CHANGE
				printf("Calling printBoard from HalController::detectChange\n");
				printf("Change has been detected, diffVector:\n");
				vectorPrint(diffVector);
				printf("diffVector size: %d\n", diffVector.size());
			#endif
			//call debounce on the change
			if(diffVector.size() == 1){
				#ifdef DEBUG_DETECT_CHANGE
					printf("Calling debounce\n");
				#endif
				db_flag = debounce(newHalVector);
			}
			diffVector.clear();
		} 
	}

	//subtract vectors to find change sensor change edge direction (rising_edge (+1) or falling_edge (-1))
	//for(int i = halVector.size() - 1; i >= 0 ; i--){
	for(int i = 0; i < halVector.size() ; i++){
		subtractVector.push_back(halVector[i] - newHalVector[i]);
	}

	#ifdef DEBUG_DETECT_CHANGE
		printf("Calling printBoard from HalController::detectChange\n");
		printf("subtract has been created, subtractVector:\n");
		vectorPrint(subtractVector);
	#endif	

	for(int x : subtractVector){
		if(x != 0){
			move.risingEdge = (x == -1)?true:false;
			move.position = {i%8, i/8};
#ifdef DEBUG_DETECT_CHANGE
			printf("Detected change on collumn %d row %d -> piece has been %s\n", 
				move.position.x, move.position.y, move.risingEdge?"placed":"removed");
			//printBoard();
#endif
		}
		i++;
	}
	halVector = newHalVector;
	#ifdef DEBUG_DETECT_CHANGE
		printf("Updated halVector:\n");
		printBoard();
	#endif
	return move;
}

void HalController::printBoard(){
	int i=0;
	std::vector<int> reverseHalVector;
	for(int x : halVector){
		reverseHalVector.push_back(x);
	}

    printf("Printing Board as seen by IOController::HalController::printBoard\n");
	for(int sensor : reverseHalVector){
		if(i%8 == 0 && i != 0){
			printf("\n");
		}

		printf("%2d: %2d, ", i, sensor);
		
		i++;
	}
	printf("\n\n");
}

std::vector<int> HalController::getHalVector() {
	// Chess::Position pos;
	// std::vector<int> halVector;

	// for (i = 0; i < 64; i++) {
	// 	pos = {i/8, i%8}
	// 	if (Chess::getPiece(pos) == NULL) {
	// 		halVector.push_back(0);
	// 	} else {
	// 		halVector.push_back(1);
	// 	}
	// }

	return halVector;
}

LedController::LedController(gpio_num_t led_strip_D0, RGBColor defaultWhite, RGBColor defaultBlack){
	this->defaultWhite = defaultWhite;
	this->defaultBlack = defaultBlack;
    this->pixels = new Adafruit_NeoPixel(64, led_strip_D0, NEO_GRB + NEO_KHZ800);
    start();
}

LedController::~LedController(){
#ifdef debug
    printf("Deconstructing IOController\n");
#endif
}

void LedController::start() {
    pixels->begin();
	pixels->show();
	pixels->clear();
    setDefaultLedVector();
	defaultLedUpdate();
	//pixels->show();
}

void LedController::setDefaultLedVector(){
	defaultLedVector.clear();
	for(int i = 0; i < 64; i++){
		int row = i/8;
		int column = i%8;
		RGBColor color;
		if(row%2 == 0){
			color = (i%2) ? defaultWhite : defaultBlack;
		}
		else{
			color = (i%2) ? defaultBlack : defaultWhite;
		}

		LED_Light light = {{column, row}, color};
		defaultLedVector.push_back(light); // Creates grid pattern
    }
}

void LedController::defaultLedUpdate(){
	ledVector.clear();
	ledVector = defaultLedVector;
	LedStrip_Output();
}

std::vector<LED_Light> LedController::getDefaultLedVector(){
	return defaultLedVector;
}



void LedController::singleLedUpdate(LED_Light newLED){
	int i = (newLED.pos.y*8)+newLED.pos.x;
	ledVector.at(i) = newLED;
}

void LedController::vectorLedUpdate(std::vector<Position> updateVector, RGBColor color){
	LED_Light light;

	for(Position square : updateVector){
		light = {square, color};
		printf("setting {%d, %d} to {%d, %d, %d}\n", square.x, square.y, color.r, color.g, color.b);
		singleLedUpdate(light);
	}
	
	LedStrip_Output();
}



/* Function:    LedStripOutput
 * Arguments:   None
 * Description: Turns on/off the board's LEDs based on the values found from ledVector
 */
/**/
void LedController::LedStrip_Output() {
	int row, column, led = 0;

	for(int i = 0; i < 64; i++){
		row = i / 8;	// 0 - 7
 		column = i % 8; // 0 - 7
		
		led = (row % 2 == 1) ? i : (((row+1) * 8) - column - 1); // Fix for LEDs vector zig-zagging
		//printf("Mapping {%2d, %2d} to led: %2d\n", column, row, led);

		//There has got to be a better way to do this -- casting the struct? -- inheriting the object from the adafruit lib?
		pixels->setPixelColor(led, pixels->Color(ledVector.at(i).rgb_color.r, ledVector.at(i).rgb_color.g, ledVector.at(i).rgb_color.b));
	}
	pixels->show();
}
