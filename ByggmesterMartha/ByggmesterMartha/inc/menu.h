/*
 * menu.h
 *
 * Created: 23.09.2020 08:59:24
 *  Author: marth
 */ 


#ifndef MENU_H_
#define MENU_H_
#include <stdio.h>

#define LINELENGTH	16
volatile char menu[8*LINELENGTH + 1];

uint8_t current_selection;
uint8_t menu_upper_bound;
uint8_t menu_lower_bound;


void menu_init();

// MENU NAVIGATION //
void menu_navigate();
void menu_go_up();
void menu_go_down();



// CHANGE MENU DATA //
void menu_home();
void menu_highscores();
void menu_game_over(uint16_t score);
void menu_play_game();

void menu_insert_item(volatile char* menu, char *element, uint8_t len, uint8_t position);

void menu_draw_item(uint8_t position);
void menu_draw();


#endif /* MENU_H_ */