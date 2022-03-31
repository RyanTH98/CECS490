#include "IOController.h"

using namespace IOController;

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
    		
		delay(10);
		//printf("I recieved %d on sensor %d\n", gpio_get_level(muxY), i);
    	newHalVector.push_back(!gpio_get_level(muxY));
		delay(15);
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
									1,1,1,0,1,1,1,1,
									0,0,0,1,0,0,0,0,
									0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,
									1,1,1,1,1,1,1,1,
									1,1,1,1,1,1,1,1,
								};
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

		//if there is a change in the sensors
		if(halVector != newHalVector){
			//make sure only one change has been made
			std::set_difference(halVector.begin(), halVector.end(), newHalVector.begin(), newHalVector.end(), 
								std::inserter(diffVector, diffVector.begin()));
			//call debounce on the change
			if(diffVector.size() == 1){
				db_flag = debounce(newHalVector);
			}
		} 
	}

	//subtract vectors to find change sensor change edge direction (rising_edge (+1) or falling_edge (-1))
	for(int i = 0; i < halVector.size(); i++){
		subtractVector[i] = halVector[i] - newHalVector[i];
	}
	

	for(int x : subtractVector){
		if(x != 0){
			move.risingEdge = (x == 1)?true:false;
			move.targetSquare = {i/8, i%8};
#ifdef debug
			printf("Detected change on collumn %d row %d -> piece has been %s\n", 
				move.targetSquare.x, move.targetSquare.y, move.risingEdge?"placed":"removed");
#endif
		}
		i++;
	}
	halVector = newHalVector;
	return move;
}