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

void clear_row(uint8_t row){
	for(uint8_t j = 0; j < 128; j++){
		write_d(0);
	}
};

void oled_goto_column(uint8_t col){
	uint8_t upper_address = col >> 4;
	uint8_t lower_address = col & 0b00001111;

	write_c(upper_address | 0b00010000);
	write_c(lower_address);
};

void oled_pos(row, col){
	oled_goto_column(col);
	oled_goto_row(row);
};

void clear_screen(){
	for(uint8_t i=0; i<8; i++){
			oled_goto_row(i);
			for(uint8_t j = 0; j < 128; j++){
				write_d(0);
				_delay_ms(5);
			}
		}
}

void oled_print(char* letter){
	for (int i = 0; i < sizeof(font8[*letter]); i++){
		uint8_t tall = font8[*letter][0];
		write_d(tall);
	} 
};


void init_oled(){
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
