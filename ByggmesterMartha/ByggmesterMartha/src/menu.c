/**
* @file
* @brief Implementation of menu library
*/

#include <stdio.h>
#include <string.h>

#include "../inc/menu.h"
#include "../inc/oled.h"
#include "../inc/statemachine.h"


void menu_init(){
	last_joystick_dir = NEUTRAL;	
	current_selection = 3;
	menu_upper_bound = 3;
	menu_lower_bound = 5;
	menu_home();
};



/**
* @brief Update specified row/item in menu
*
* @param[in] row Current row to draw on oled
*
* @return void
*/
void menu_draw_item(uint8_t row){
	//Clear row and move pointer
	oled_clear_row(row);
	oled_pos(row, 0);

	// Retrieve correct menu data
	char _item[LINELENGTH];
	memcpy(&_item[0], &menu[row*LINELENGTH], LINELENGTH);
	_item[LINELENGTH] = '\0';
	
	// Add oled pointer to the "selected line"
	if(current_selection == row){
		_item[0] = 42;
	}
	else{
		_item[0] = 32;
	}
	
	// Write data row to oled
	for(uint8_t i = 0; i<LINELENGTH; i++){
		oled_print_char(&_item[i]);
	}
}


void menu_draw(){
	// Draw every row of the oled
	for (uint8_t row = 0; row < 8; row++){
		menu_draw_item(row);
	}
	
}



/**
* @brief Move menu selector one up if possible
*
* @return void
*/
void menu_go_up(){
	if (current_selection > menu_upper_bound){ //Upper bound){
		current_selection -= 1;
	}
}

/**
* @brief Move menu selector one down if possible
*
* @return void
*/
void menu_go_down(){
	if (current_selection < menu_lower_bound){ //Lower Bound){
		current_selection += 1;
	}
	
}

void menu_navigate(){
	enum Joystick_dir direction = get_joystick_dir(); 
	
	// Check for a change in joystick direction to make it easy to control menu selector
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
}




/**
* @brief Abstraction for changing single items/menu-rows
*
* @param[in] menu Pointer to menu to be overwritten
* @param[in] element Pointer to buffer containing content of the new menu row
* @param[in] len Length of the buffer to be written
* @param[in] position What row in menu to be overwritten
*
* @return void
*/
void menu_insert_item(volatile char* menu, char *element, uint8_t len, uint8_t position){
	memcpy((char*)&menu[position*LINELENGTH], element, len);
	memset((char*)&menu[(position*LINELENGTH) + len], 32, LINELENGTH-len);	// 32 is ascii for SPACE <3
}

void menu_home(){
	volatile char* menu_pointer = &menu[0];
	//Title
	char title[LINELENGTH] = "    WELCOME!";
	menu_insert_item(menu_pointer, &title[0], strlen(title), 0);
	
	// Menu options, other rows are set to "" 
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

void menu_developers(){
	volatile char* menu_pointer = &menu[0];
	char title[LINELENGTH] = "  DEVELOPERS!";
	menu_insert_item(menu_pointer, &title[0], strlen(title), 0);
	
	// Menu options, other rows are set to "" 
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
	
	// Menu options, other rows are set to "" 
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
	
	// Menu options, other rows are set to "" 
	char option0[LINELENGTH] = "  PING PONG! ";
	char option1[LINELENGTH];
	sprintf(&option1[0], " Score %u", score);
	
	menu_insert_item(menu_pointer, &option0[0], strlen(option0), 0);
	menu_insert_item(menu_pointer, "", 0, 1);
	menu_insert_item(menu_pointer, "", 0, 2);
	menu_insert_item(menu_pointer, "", 0, 3);
	menu_insert_item(menu_pointer, &option1[0], strlen(option1), 4);
	menu_insert_item(menu_pointer, "", 0, 5);
	menu_insert_item(menu_pointer, "", 0, 6);
	menu_insert_item(menu_pointer, "", 0, 7);
}