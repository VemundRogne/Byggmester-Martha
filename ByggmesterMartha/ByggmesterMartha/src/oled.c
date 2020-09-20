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

void write_c(uint8_t command){
	ext_oled_cmd[0] = command;
}

void write_d(uint8_t data){
	ext_oled_data[0] = data;
}

void oled_goto_row(uint8_t row){
	write_c(0xB0 | row);
};


void oled_goto_column(uint8_t col){
	uint8_t upper_address = col >> 4;
	uint8_t lower_address = col & 0b00001111;

	write_c(upper_address | 0b00010000);
	write_c(lower_address);
};

void oled_pos(row, col){
	oled_goto_row(row);
	oled_goto_column(col);
};


void oled_clear_row(row){
	oled_goto_row(row);
	for(uint8_t j = 0; j < 128; j++){
		write_d(0);
		_delay_ms(5);
	}
};

void oled_fill_row(row){
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

void oled_print_char(char* letter){
	uint8_t offset = 32;
	for (int i = 0; i < 8; i++){
		uint8_t data = pgm_read_byte(&(font8[*letter - offset][i]));
		write_d(data);
	} 
};

void oled_print_string(char* char_pointer){
	while (*char_pointer != '\0'){
		oled_print_char(char_pointer);
		char_pointer++;
		_delay_ms(5);
	}
};


void oled_init(){
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
