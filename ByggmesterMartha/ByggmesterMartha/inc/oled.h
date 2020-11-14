/**
* @file
* @brief Library for communicating and controlling oled
*/

#ifndef OLED_H_
#define OLED_H_
#include <stdio.h>

#define OLED_COMMAND_ADDRESS 0x1000
#define OLED_DATA_ADDRESS 0x1200


/**
* @brief Initialize oled
*
* @return void
*/
void oled_init();


/**
* @brief Set position of oled pointer
*
* @param[in] row Desired row of oled pointer
* @param[in] col Desired col of oled pointer
*
* @return uint8_t Row moved to
*/
uint8_t oled_pos(uint8_t row, uint8_t col);


/**
* @brief Clear entire oled screen
*
* @return void
*/
void oled_clear();

/**
* @brief Clear a specified row of the oled
*
* @param[in] row Desired row to clear
*
* @return void
*/
void oled_clear_row(uint8_t row);

/**
* @brief Print a char from fonts.h to current oled pointer position
*
* @param[in] letter Pointer to array containing character data
*
* @return void
*/
void oled_print_char(char* letter);

/**
* @brief Display a string on the oled
*
* @param[in] char_pointer Pointer to start of string
* @param[in] current_oled_row Row to write string on
*
* @return void
*/
void oled_print_string(char* char_pointer, uint8_t current_oled_row);


#endif /* OLED_H_ */