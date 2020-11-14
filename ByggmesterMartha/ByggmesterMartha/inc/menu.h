/**
* @file
* @brief Library for updating menu on oled
*/

#ifndef MENU_H_
#define MENU_H_
#define LINELENGTH	16

#include <stdio.h>
#include "funkyboard.h"

volatile char menu[8*LINELENGTH + 1];

enum Joystick_dir last_joystick_dir;
uint8_t current_selection;
uint8_t menu_upper_bound;
uint8_t menu_lower_bound;


/**
* @brief Initialize menu to home
*
* @return void
*/
void menu_init();

/**
* @brief Navigate menu based on joystick direction
*
* @return void
*/
void menu_navigate();



/**
* @brief Update menu to home menu
*
* @return void
*/
void menu_home();

/**
* @brief Update menu to developers
*
* @return void
*/
void menu_developers();

/**
* @brief Update menu to game over
*
* @param[in] score Score obtained in game
*
* @return void
*/
void menu_game_over(uint16_t score);

/**
* @brief Update menu to play game
*
* @param[in] score Current score in game
*
* @return void
*/
void menu_play_game(uint16_t score);

/**
* @brief Draw current menu on oled
*
* @return void
*/
void menu_draw();



#endif /* MENU_H_ */