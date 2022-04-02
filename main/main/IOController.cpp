#include "IOController.h"

using namespace IOController;

#define DEBUG


#ifdef DEBUG
void vectorPrint(std::vector<int> vec){
	int i = 0;
    printf("Printing Board as seen by vectorPrint\n");
	for(int sensor : vec){
		if(i%8 == 0 && i != 0){
			printf("\n");
		}

		printf("%2d: %2d, ", i, sensor);
		
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

    unsigned short column;
	unsigned short row;
	for(unsigned short i = 0; i < 64; i++){
		row = i % 8;
		column = i / 8;

   		gpio_set_level(muxA, row & 0x01);
   		gpio_set_level(muxB, (row>>1)&0x01);
   		gpio_set_level(muxC, (row>>2)&0x01);
   		gpio_set_level(decoderA0, column&0x01);
   		gpio_set_level(decoderA1, (column>>1)&0x01);
   		gpio_set_level(decoderA2, (column>>2)&0x01);
    		
		delay(25);
		//printf("I recieved %d on sensor %d\n", gpio_get_level(muxY), i);
    	newHalVector.push_back(!gpio_get_level(muxY));
		//delay(25);
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

bool HalController::checkStartingPosition(){
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
	#ifdef DEBUG
			printf("Calling printBoard from HalController::checkStartingPosition\n");
			//printBoard();
			printf("halVector:\n");
			vectorPrint(halVector);
			printf("checkVector:\n");
			vectorPrint(checkVector);
			printf("Equality: %d\n\n\n", checkVector == halVector);
	#endif
	return checkVector == halVector;
}

bool HalController::debounce(std::vector<int> debounceHal){
	for(int i = 0; i < DEBOUNCE_COUNT; i++){
		if(debounceHal != scan()){
			return false;
		}
	}
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
		#ifdef DEBUG
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
			#ifdef DEBUG
				printf("Calling printBoard from HalController::detectChange\n");
				printf("Change has been detected, diffVector:\n");
				vectorPrint(diffVector);
				printf("diffVector size: %d\n", diffVector.size());
			#endif
			//call debounce on the change
			if(diffVector.size() == 1){
				#ifdef DEBUG
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

	#ifdef DEBUG
		printf("Calling printBoard from HalController::detectChange\n");
		printf("subtract has been created, subtractVector:\n");
		vectorPrint(subtractVector);
	#endif	

	for(int x : subtractVector){
		if(x != 0){
			move.risingEdge = (x == -1)?true:false;
			move.targetSquare = {i%8, i/8};
#ifdef DEBUG
			printf("Detected change on collumn %d row %d -> piece has been %s\n", 
				move.targetSquare.x, move.targetSquare.y, move.risingEdge?"placed":"removed");
			//printBoard();
#endif
		}
		i++;
	}
	halVector = newHalVector;
	#ifdef DEBUG
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
    setDefaultLights();
    LedStrip_Output();
}

void LedController::setDefaultLights(){
	for(int i = 0; i < 64; i++){
    	ledVector.push_back({{i/8, i%8}, (i%2)?defaultWhite:defaultBlack});
    }
}

void LedController::singleLedUpdate(LED_Light newLED){
    for(LED_Light oldLED : ledVector){
		if(oldLED.pos.x == newLED.pos.x && oldLED.pos.y == newLED.pos.y){
			oldLED.rgb_color = newLED.rgb_color;
			continue;
		}
	}
}

void LedController::vectorLedUpdate(std::vector<LED_Light> updateVector){
	for(LED_Light led : updateVector){
		singleLedUpdate(led);
	}
}



/* Function:    LedStripOutput
 * Arguments:   None
 * Description: Turns on/off the board's LEDs based on the values found from ledVector
 */
/**/
void LedController::LedStrip_Output() {
	int row, column, led;
	for(int i = 0; i < 64; i++){
		row = i % 8;
 		column = i / 8;
		led = (column%2 == 0) ? i : ((column + 1) * 64 - row - 1);
		//There has got to be a better way to do this -- casting the struct? -- inheriting the object from the adafruit lib?
		pixels->setPixelColor(led, pixels->Color(ledVector[i].rgb_color.r, ledVector[i].rgb_color.g, ledVector[i].rgb_color.b));
	}
    pixels->show();   // Send the updated pixel colors to the hardware.
}

