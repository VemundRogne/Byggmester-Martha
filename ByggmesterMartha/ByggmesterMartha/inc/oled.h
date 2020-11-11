/*
 * oled.h
 *
 * Created: 19.09.2020 11:20:18
 *  Author: marth
 */ 


#ifndef OLED_H_
#define OLED_H_
#include <stdio.h>

#define OLED_COMMAND_ADDRESS 0x1000
#define OLED_DATA_ADDRESS 0x1200


//Initializer
void oled_init();

//Moving position pointer
uint8_t oled_pos(uint8_t row, uint8_t col);

//Clear and fills
void oled_clear();
void oled_clear_row(uint8_t row);

//Character and strings
void oled_print_char(char* letter);
void oled_print_string(char* char_pointer, uint8_t current_oled_row);


#endif /* OLED_H_ */