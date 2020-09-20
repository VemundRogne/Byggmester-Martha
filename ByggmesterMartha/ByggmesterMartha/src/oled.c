/*
 * oled.c
 *
 * Created: 19.09.2020 11:20:34
 *  Author: marth
 */ 

#include <util/delay.h>
#include "../inc/oled.h"
#include "../inc/fonts.h"

volatile char *ext_oled_cmd = OLED_COMMAND_ADDRESS;
volatile char *ext_oled_data = OLED_DATA_ADDRESS;


//Initializer
void oled_init(){
	memset(&inversion_mask[0], 0, 8);
	inversion_mask[2] = 1;
	write_c(0xae); // display off
	write_c(0xa1); //segment remap
	write_c(0xda); //common pads hardware: alternative
	write_c(0x12);
	write_c(0xc8); //common output scan direction:com63~com0
	write_c(0xa8); //multiplex ration mode:63
	write_c(0x3f);
	write_c(0xd5); //display divide ratio/osc. freq. mode
	write_c(0x80);
	write_c(0x81); //contrast control
	write_c(0x50);
	write_c(0xd9); //set pre-charge period
	write_c(0x21);
	write_c(0x20); //Set Memory Addressing Mode
	write_c(0x02);
	write_c(0xdb); //VCOM deselect level mode
	write_c(0x30);
	write_c(0xad); //master configuration
	write_c(0x00);		
	write_c(0xa4); //out follows RAM content
	write_c(0xa6); //set normal display
	write_c(0xaf); // display on
	write_c(0x10);
}


//Abstraction for writing commands and data
void write_c(uint8_t command){
	ext_oled_cmd[0] = command;
}

void write_d(uint8_t data){
	ext_oled_data[0] = data;
}


//Moving position pointer
void oled_goto_row(uint8_t row){
	write_c(0xB0 | row);
};

void oled_goto_column(uint8_t col){
	uint8_t upper_address = col >> 4;
	uint8_t lower_address = col & 0b00001111;

	write_c(upper_address | 0b00010000);
	write_c(lower_address);
};

uint8_t oled_pos(uint8_t row, uint8_t col){
	oled_goto_row(row);
	oled_goto_column(col);
	return row;
};


//Clear and fills
void oled_clear_row(uint8_t row){
	oled_goto_row(row);
	for(uint8_t j = 0; j < 128; j++){
		write_d(0);
		//_delay_ms(5);
	}
};

void oled_fill_row(uint8_t row){
	oled_goto_row(row);
	for(uint8_t j = 0; j < 128; j++){
		write_d(255);
		_delay_ms(5);
	}
};

void oled_clear(){
	for(uint8_t row=0; row<8; row++){
		oled_clear_row(row);
	}
};


//Character and strings
void oled_print_char(char* letter, uint8_t current_oled_row){
	uint8_t offset = 32;
	for (int i = 0; i < 8; i++){
		uint8_t data = pgm_read_byte(&(font8[*letter - offset][i]));
		if (inversion_mask[current_oled_row]){
			data = 255 - data;
		}
		write_d(data);
	} 
};

void oled_print_string(char* char_pointer, uint8_t current_oled_row){
	while (*char_pointer != '\0'){
		oled_print_char(char_pointer, current_oled_row);
		char_pointer++;
		_delay_ms(5);
	}
};

void oled_change_row(char* char_pointer, uint8_t current_oled_row){
	oled_clear_row(current_oled_row);
	oled_pos(current_oled_row, 0);
	oled_print_string(char_pointer, current_oled_row);
}


void oled_test(){
	oled_clear();

	for (uint8_t j = 0; j < 8; j++){
		uint8_t current_oled_row = oled_pos(j, 0);
		//oled_goto_column(0);
		//oled_goto_row(j);
    	oled_print_string(" HELLO WORLD! ", current_oled_row);
    	_delay_ms(5);
	}

};

