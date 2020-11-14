/**
* @file
* @brief Implementation of the statemachine library
*/

#include <stdio.h>

#include "../inc/statemachine.h"
#include "../inc/menu.h"
#include "../inc/game.h"
#include "../inc/can.h"
#include "../inc/funkyboard.h"
#include "../inc/oled.h"


void statemachine_init(){
	current_state = HOME_MENU;
}

void enter_home_menu(){
	current_state = HOME_MENU;
	menu_home();
	
	current_selection = 3;
	menu_upper_bound = 3;
	menu_lower_bound = 5;
}

void enter_developers_menu(){
	current_state = DEVELOPERS;
	menu_developers();

	current_selection = 5;
	menu_lower_bound = 5;
	menu_upper_bound = 5;
}

void enter_game_over(uint16_t score){
	current_state = GAME_OVER;
	menu_game_over(score);
	
	current_selection = 5;
	menu_lower_bound = 5;
	menu_upper_bound = 5;
}

void enter_play_game(uint16_t score){
	menu_play_game(score);
	current_state = PLAY_GAME;
	current_selection = 8;
}

void enter_initializing(){
	current_state = INITIALIZING;
}

/**
* @brief Transition function to END_GAME
*
* @return void
*/

void enter_end_game(){
	current_state = END_GAME;
}

void statemachine_menu_selection(){
	switch(current_state){
		case HOME_MENU:
			switch(current_selection){
				// Enter play game, wait while initializing
				case 3: {
					struct can_msg init_regulator_msg;
					init_regulator_msg.ID = 138;
					init_regulator_msg.len = 1;
					init_regulator_msg.data[0] = 1;
				
					can_transmit_message(&init_regulator_msg);
					oled_clear();
					enter_initializing();
				}
					break;

				case 4:
					enter_developers_menu();
					break;
					
				case 5:
					enter_end_game();
					break;
			}
			break;

		case DEVELOPERS:
			switch(current_selection){
				default:
					enter_home_menu();
					break;
			}
			break;
		case GAME_OVER:
			switch(current_selection){
				default:
					enter_home_menu();
					break;
			}
			break;
		case PLAY_GAME:
			switch(current_selection){
				default:
					break;
			}
			break;
		default:
			break;
	}
}


void statemachine_execute_current_state(){
	switch(current_state){
		case HOME_MENU:
			menu_draw((char*)&menu[0]);
			menu_navigate();
			break;

		case DEVELOPERS:
			menu_draw((char*)&menu[0]);
			menu_navigate();
			break;
			
		case GAME_OVER:
			menu_draw((char*)&menu[0]);
			menu_navigate();
			break;
			
		case PLAY_GAME:
			menu_play_game(score_count);
			menu_draw((char*)&menu[0]);
			break;
		
		case INITIALIZING:
			oled_print_string(" INITIALIZING", 4);
			break;
			
		case END_GAME:
			oled_clear();
			break;
	}
}