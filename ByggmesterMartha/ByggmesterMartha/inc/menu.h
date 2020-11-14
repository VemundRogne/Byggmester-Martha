
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


void menu_init();

// Move menu pointer
void menu_navigate();

// CHANGE MENU DATA //
void menu_home();
void menu_highscores();
void menu_game_over(uint16_t score);
void menu_play_game(uint16_t score);

// Draw current menu to oled
void menu_draw();



#endif /* MENU_H_ */