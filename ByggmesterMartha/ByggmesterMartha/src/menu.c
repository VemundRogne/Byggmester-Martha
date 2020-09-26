/*
 * menu.c
 *
 * Created: 23.09.2020 08:59:35
 *  Author: marth
 */ 

#include "../inc/menu.h"
#include "../inc/oled.h"
#include "../inc/mmi.h"
#include "../inc/statemachine.h"
#include <string.h>

enum Joystick_dir last_joystick_dir = NEUTRAL;

void menu_init(){
	memset(&inversion_mask[0], 0, 8);
	oled_clear();
	
	current_selection = 3;
	menu_upper_bound = 3;
	menu_lower_bound = 5;
};

void menu_insert_item(char* menu, char *element, uint8_t len, uint8_t position){
	memcpy(&menu[position*LINELENGTH], element, len);
	memset(&menu[(position*LINELENGTH) + len], 32, LINELENGTH-len);	// 32 is ascii for SPACE <3
}

void menu_draw_item(uint8_t position, char *menu){
	oled_clear_row(position);
	oled_pos(position, 0);
	char _item[LINELENGTH];
	
	memcpy(&_item[0], &menu[position*LINELENGTH], LINELENGTH);
	_item[LINELENGTH] = '\0';
	
	if(current_selection == position){
		_item[0] = 42;
	}
	else{
		_item[0] = 32;
	}
	
	for(uint8_t i = 0; i<LINELENGTH; i++){
		oled_print_char(&_item[i], position);
	}
}

void menu_draw(char *menu){
	memset(&inversion_mask[0], 0, 8);
	//inversion_mask[current_selection] = 1;
	for (uint8_t i = 0; i < 8; i++){
		menu_draw_item(i, menu);
	}
	
};

void menu_go_up(){
	if (current_selection > menu_upper_bound){ //Upper bound){
		current_selection -= 1;
	}
};

void menu_go_down(){
	if (current_selection < menu_lower_bound){ //Lower Bound){
		current_selection += 1;
	}
	
};

void menu_navigate(){
	enum Joystick_dir direction = get_joystick_dir(); 
	
	if ((direction == DOWN) && (last_joystick_dir != DOWN)) {
		menu_go_down();
	}
	if ((direction == UP) && (last_joystick_dir != UP)){
		menu_go_up();
	}
	if ((direction == LEFT) && (last_joystick_dir != LEFT)){
		statemachine_handle_menu_execute();
	}
	
		
	last_joystick_dir = direction;
};

uint8_t menu_select(){
	printf("HEEEEEEEEEI <3333");
};

void menu_home(){
	//Title
	char title[LINELENGTH] = "    WELCOME!";
	menu_insert_item(&home_menu, &title, strlen(title), 0);
	
	//Options
	char option1[LINELENGTH] = " Play ping pong";
	char option2[LINELENGTH] = " High scores";
	char option3[LINELENGTH] = " End game";
	
	menu_insert_item(&home_menu, NULL, 0, 1);
	menu_insert_item(&home_menu, NULL, 0, 2);
	menu_insert_item(&home_menu, &option1, strlen(option1), 3);
	menu_insert_item(&home_menu, &option2, strlen(option2), 4);
	menu_insert_item(&home_menu, &option3, strlen(option3), 5);
	menu_insert_item(&home_menu, NULL, 0, 6);
	menu_insert_item(&home_menu, NULL, 0, 7);
};

void menu_highscores(){
	char title[LINELENGTH] = "  HIGH SCORE!";
	menu_insert_item(&hs_menu, &title, strlen(title), 0);
	
	//Options
	char option1[LINELENGTH] = " Oskar 100";
	char option2[LINELENGTH] = " Vemund 50";
	char option3[LINELENGTH] = " Martha 3";
	char option4[LINELENGTH] = " Back";
	
	menu_insert_item(&hs_menu, NULL, 0, 1);
	menu_insert_item(&hs_menu, &option1, strlen(option1), 2);
	menu_insert_item(&hs_menu, &option2, strlen(option2), 3);
	menu_insert_item(&hs_menu, &option3, strlen(option3), 4);
	menu_insert_item(&hs_menu, &option4, strlen(option4), 5);
	menu_insert_item(&hs_menu, NULL, 0, 6);
	menu_insert_item(&hs_menu, NULL, 0, 7);
};
