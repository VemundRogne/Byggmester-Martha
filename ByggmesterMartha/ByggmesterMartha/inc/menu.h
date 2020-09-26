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
volatile char home_menu[8*LINELENGTH + 1];
volatile char hs_menu[8*LINELENGTH + 1];

uint8_t current_selection;

void menu_init();

void menu_insert_item(char* menu, char *element, uint8_t len, uint8_t position);

void menu_draw_item(uint8_t position, char *menu);

void menu_go_up();

void menu_go_down();

void menu_navigate();

uint8_t menu_select(); //Må kanskje gjøres om til en interrupt.

void menu_home();

void menu_highscores();

void menu_draw(char *menu);

void menu_test();

//void menu_highscorse(); //TODO
//void menu_play_game(); //TODO


#endif /* MENU_H_ */