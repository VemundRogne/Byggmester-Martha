/*
 * oled.c
 *
 * Created: 19.09.2020 11:20:34
 *  Author: marth
 */ 

#include "../inc/oled.h"

volatile char *ext_oled_cmd = OLED_COMMAND_ADDRESS;
volatile char *ext_oled_data = OLED_DATA_ADDRESS;

void write_c(uint8_t command){
	ext_oled_cmd[0] = command;
}

void write_d(uint8_t data){
	ext_oled_data[0] = data;
}

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
}
