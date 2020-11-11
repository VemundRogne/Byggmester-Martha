/*
 * menu.c
 *
 * Created: 23.09.2020 08:59:35
 *  Author: marth
 */ 

#include <stdio.h>

#include "../inc/menu.h"
#include "../inc/oled.h"
#include "../inc/funkyboard.h"
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

void menu_insert_item(volatile char* menu, char *element, uint8_t len, uint8_t position){
	memcpy((char*)&menu[position*LINELENGTH], element, len);
	memset((char*)&menu[(position*LINELENGTH) + len], 32, LINELENGTH-len);	// 32 is ascii for SPACE <3
}

void menu_draw_item(uint8_t row){
	oled_clear_row(row);
	oled_pos(row, 0);
	char _item[LINELENGTH];
	
	memcpy(&_item[0], &menu[row*LINELENGTH], LINELENGTH);
	_item[LINELENGTH] = '\0';
	
	if(current_selection == row){
		_item[0] = 42;
	}
	else{
		_item[0] = 32;
	}
	
	for(uint8_t i = 0; i<LINELENGTH; i++){
		oled_print_char(&_item[i]);
	}
}

void menu_draw(){
	for (uint8_t row = 0; row < 8; row++){
		menu_draw_item(row);
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
		statemachine_menu_selection();
	}
	
		
	last_joystick_dir = direction;
};


void menu_home(){
	volatile char* menu_pointer = &menu[0];
	//Title
	char title[LINELENGTH] = "    WELCOME!";
	menu_insert_item(menu_pointer, &title[0], strlen(title), 0);
	
	//Options
	char option1[LINELENGTH] = " Play ping pong";
	char option2[LINELENGTH] = " Developers";
	char option3[LINELENGTH] = " End game";
	
	menu_insert_item(menu_pointer, "", 0, 1);
	menu_insert_item(menu_pointer, "", 0, 2);
	menu_insert_item(menu_pointer, &option1[0], strlen(option1), 3);
	menu_insert_item(menu_pointer, &option2[0], strlen(option2), 4);
	menu_insert_item(menu_pointer, &option3[0], strlen(option3), 5);
	menu_insert_item(menu_pointer, "", 0, 6);
	menu_insert_item(menu_pointer, "", 0, 7);
};

void menu_highscores(){
	volatile char* menu_pointer = &menu[0];
	char title[LINELENGTH] = "  DEVELOPERS!";
	menu_insert_item(menu_pointer, &title[0], strlen(title), 0);
	
	//Options
	char option1[LINELENGTH] = " Oskar";
	char option2[LINELENGTH] = " Vemund";
	char option3[LINELENGTH] = " Martha";
	char option4[LINELENGTH] = " Back";
	
	menu_insert_item(menu_pointer, "", 0, 1);
	menu_insert_item(menu_pointer, &option1[0], strlen(option1), 2);
	menu_insert_item(menu_pointer, &option2[0], strlen(option2), 3);
	menu_insert_item(menu_pointer, &option3[0], strlen(option3), 4);
	menu_insert_item(menu_pointer, &option4[0], strlen(option4), 5);
	menu_insert_item(menu_pointer, "", 0, 6);
	menu_insert_item(menu_pointer, "", 0, 7);
};


void menu_game_over(uint16_t score){
	volatile char* menu_pointer = &menu[0];
	char title[LINELENGTH] = "  GAME OVER!";
	menu_insert_item(menu_pointer, &title[0], strlen(title), 0);
	
	//Options
	//fprintf(" Score %u", score);
	//char option2[LINELENGTH];
	//sprintf(&score, "Score %u", score);
	char option2[LINELENGTH];
	sprintf(&option2[0], " Score %u", score);
	char option3[LINELENGTH] = " Well done! ";
	char option4[LINELENGTH] = " Play again?";
	
	menu_insert_item(menu_pointer, "", 0, 1);
	menu_insert_item(menu_pointer, "", 0, 2);
	menu_insert_item(menu_pointer, &option2[0], strlen(option2), 3);
	menu_insert_item(menu_pointer, &option3[0], strlen(option3), 4);
	menu_insert_item(menu_pointer, &option4[0], strlen(option4), 5);
	menu_insert_item(menu_pointer, "", 0, 6);
	menu_insert_item(menu_pointer, "", 0, 7);
}

void menu_play_game(uint16_t score){
	volatile char* menu_pointer = &menu[0];
	

	char line0[LINELENGTH] = "  PING PONG! ";
	
	char line1[LINELENGTH];
	sprintf(&line1[0], " Score %u", score);
	
	
	
	menu_insert_item(menu_pointer, &line0[0], strlen(line0), 0);
	menu_insert_item(menu_pointer, "", 0, 1);
	menu_insert_item(menu_pointer, "", 0, 2);
	menu_insert_item(menu_pointer, "", 0, 3);
	menu_insert_item(menu_pointer, &line1[0], strlen(line1), 4);
	menu_insert_item(menu_pointer, "", 0, 5);
	menu_insert_item(menu_pointer, "", 0, 6);
	menu_insert_item(menu_pointer, "", 0, 7);
}