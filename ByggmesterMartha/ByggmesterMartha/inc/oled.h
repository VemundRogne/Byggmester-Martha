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

uint8_t inversion_mask[8];

//Initializer
void oled_init();

//Abstraction for writing commands and data
void write_c(uint8_t command);
void write_d(uint8_t data);

//Moving position pointer
void oled_goto_row(uint8_t row);
void oled_goto_column(uint8_t col);
uint8_t oled_pos(uint8_t row, uint8_t col);

//Clear and fills
void oled_clear();
void oled_clear_row(uint8_t row);
void oled_fill_row(uint8_t row);    //Denne øydela alt

//Character and strings
void oled_print_char(char* letter, uint8_t current_oled_row);
void oled_print_string(char* char_pointer, uint8_t current_oled_row);

//Test function
void oled_test();

// ----- TO BE IMPLEMENTED ----- 
//void oled_home();
//MENY?


#endif /* OLED_H_ */