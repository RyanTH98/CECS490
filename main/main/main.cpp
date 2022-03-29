#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include "chess.h"
#define input  1
#define output 0
#define matrix_size 8 //nxn matix

//mux
gpio_num_t muxA = GPIO_NUM_5;
gpio_num_t muxB = GPIO_NUM_18;
gpio_num_t muxC = GPIO_NUM_19;
gpio_num_t muxEn_n = GPIO_NUM_2;
gpio_num_t muxY = GPIO_NUM_4;
unsigned short mux_counter = 0;

//decoder
gpio_num_t decoderA0 = GPIO_NUM_27;
gpio_num_t decoderA1 = GPIO_NUM_26;
gpio_num_t decoderA2 = GPIO_NUM_25;
gpio_num_t decoderEn = GPIO_NUM_33;
unsigned short decoder_counter = 0;

//led
gpio_num_t led_strip_D0 = GPIO_NUM_23;
Adafruit_NeoPixel pixels(matrix_size*matrix_size, led_strip_D0, NEO_GRB + NEO_KHZ800);

//Just to make life easier
gpio_num_t input_pins[]  = { muxY };
gpio_num_t output_pins[] = { muxA, muxB, muxC, muxEn_n, decoderA0, decoderA1, decoderA2, decoderEn, led_strip_D0 };

long binary_out[64] = {0};

const int scan_delay = 1;
int timer_flag = 0;
/*
enum Position {
	a1, b1, c1, d1, e1, f1, g1, h1,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a8, b8, c8, d8, e8, f8, g8, h8, 
	offBoard
};
enum GameStyle{OVER_BOARD, AI, REMOTE};
enum Color{WHITE, BLACK};
*/
//function prototypes
void Set   (int);
void delay_1ms (int);
void GPIO_Init  (int);
void Scan_Hall  (void);
void Serial_Output(void);
void LedStrip_Output(void);
//class ChessBoard;
//class ChessPiece;
//class King;
//class Queen;
//class Bishop;
//class Rook;
//class Knight;
//class Pawn;


extern "C" void app_main(){
	printf("CODE BEGIN\n");
	
	GPIO_Init(input);
	GPIO_Init(output);

	gpio_set_level(decoderEn, 1);
	gpio_set_level(muxEn_n, 0);
	
   	pixels.begin(); 
	Chess::Board board();
    while(1) {
		//delay(1000);
        Scan_Hall();
		Serial_Output();
		//LedStrip_Output();
    	}
}
/*
class Player{
	private:
	Color color;
	Position position = offBoard;
	King king(position, color);
	
	public:
	Player(Color color){
		this->color = color;
		//king = King(a4);
	}
	
};

class ChessPiece{
	private:
	bool active, is_in_starting_position;
	Color color;
	Position position;
	Position legal_moves[16] = {};

	public:
	bool getActive(){
		return active;
	}
	void setActive(bool active){
		this->active = active;
	}
	bool getIsInStartingPosition(){
		return is_in_starting_position;	
	}
	void setIsInStartingPostion(bool is_in_starting_position){
		this->is_in_starting_position = is_in_starting_position;
	}
	Position getPosition(){
		return position;
	}
	void setPosition(Position position){
		this->position = position;
	}
	
	ChessPiece(Position position, Color color){
		active = true;
		is_in_starting_position = true;
		this->position = position;
		this->color = color;
	}
	~ChessPiece(){
		//std::cout << "Destroying Chess Piece\n";
	}
};


class King: public ChessPiece{
	private:
	bool check;
	bool leftCastle;
	bool rightCastle;

	public:
	void updateLegalMoves(){
			
	}

	//1: check if king is in check
	//2: set check variable
	//3: return its value
	bool checkCheck(){
		return check;
	}
	
	//1: check if king can castle
	//2: set left and right castle variable
	void setCastle(){
	
	}
	bool getLeftCastle(){
		return leftCastle;
	}
	bool getRightCastle(){
		return rightCastle;
	}

	King(Position position, Color color): ChessPiece{position, color}
	{
		check = false;
		leftCastle = false;
		rightCastle = false;
	};
};

class Queen: public ChessPiece{

	public:
	void updateLegalMoves(){
			
	}
	
	Queen(Position position): ChessPiece(position){
		
	};
};

class Bishop: public ChessPiece{

	public:
	void updateLegalMoves(){
			
	}
	
	Bishop(Position position): ChessPiece(position){
		
	};
};


class Rook: public ChessPiece{
	private:
	bool castle;
	public:
	void updateLegalMoves(){
			
	}

	//1: check if castling is legal
	//2: set castle variable
	//3: return its value
	bool checkCastle(){
		return castle;
	}
	
	Rook(Position position): ChessPiece(position){
		castle = false;	
	};
};


class Pawn: public ChessPiece{
	private:
	bool enPasant, doubleMove;
	public:
	void updateLegalMoves(){
			
	}
	//1: check if enpasant is legal
	//2: set enpasant variable
	//3: return its value
	bool checkEnPasant(){
		return enPasant;
	}

	//1: check if double move is legal
	//2: set doubleMove variable
	//3: return its value
	bool checkDoubleMove(){
		return doubleMove;
	}
	
	Pawn(Position position): ChessPiece(position){
		enPasant = false;
		doubleMove = true;	
	};
};
*/

void delay_1ms(int duration) { vTaskDelay(duration / portTICK_PERIOD_MS); }

void LedStrip_Output(void) {
	int row, column, led;
	pixels.clear(); // Set all pixel colors to 'off'

  	for(int i=0; i<matrix_size*matrix_size; i++) {
		row = i % matrix_size;
		column = i / matrix_size;
					
    			
		led = (column%2 == 0)?i:((column+1)*matrix_size - row - 1);
		

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

void Serial_Output(void){	
	for(int i = 0; i < (sizeof(binary_out)/sizeof(binary_out[1])); i++){
		printf("%3d: %lu,    ", i, binary_out[i]);
		if((i+1) % 8 == 0){
			printf("\n");
		}
	}
	printf("\n\n");
}

void Scan_Hall(void) {
	unsigned short column;
	unsigned short row;
	for(unsigned short i = 0; i < matrix_size * matrix_size; i++){
		row = i % matrix_size;
		column = i / matrix_size;

   		gpio_set_level(muxA, row & 0x01);
   		gpio_set_level(muxB, (row>>1)&0x01);
   		gpio_set_level(muxC, (row>>2)&0x01);
   		gpio_set_level(decoderA0, column&0x01);
   		gpio_set_level(decoderA1, (column>>1)&0x01);
   		gpio_set_level(decoderA2, (column>>2)&0x01);
    		
		delay(10);
		//printf("I recieved %d on sensor %d\n", gpio_get_level(muxY), i);
    		binary_out[i] =  !gpio_get_level(muxY);
		delay(15);
	}
}

void GPIO_Init(int direction) {
    gpio_num_t pin;
    char size  = (direction ? sizeof(input_pins ) : sizeof(output_pins)) / sizeof(int);
    
    for (int i = 0; i < size; i++) {
        pin = direction ? input_pins[i] : output_pins[i];
        
        gpio_reset_pin(pin);
        gpio_set_direction(pin, ( direction ? GPIO_MODE_INPUT : GPIO_MODE_OUTPUT ));
    }
    
}
