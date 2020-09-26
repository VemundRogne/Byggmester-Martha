/*
 * statemachine.c
 *
 * Created: 26/09/2020 13:44:03
 *  Author: 
 */ 

#include <stdio.h>

#include "../inc/statemachine.h"
#include "../inc/menu.h"


void init_statemachine(){
	current_state = HOME_MENU;
}


void statemachine_handle_menu_execute(){
	switch(current_state){
		case HOME_MENU:
			switch(current_selection){
				case 4:
					enter_highscore_menu();
					break;
			}
			break;

		case HIGHSCORE:
			switch(current_selection){
				default:
					current_state = HOME_MENU;
					current_selection = 3;
					menu_upper_bound = 3;
					menu_lower_bound = 5;
					break;
			}
			break;

		default:
			break;
	}
}

void enter_highscore_menu(){
	current_state = HIGHSCORE;

	menu_highscores();

	current_selection = 5;
	menu_lower_bound = 5;
	menu_upper_bound = 5;
}


void statemachine_execute_current_state(){
	switch(current_state){
		case HOME_MENU:
			menu_draw(&home_menu);
			menu_navigate();
			break;

		case HIGHSCORE:
			menu_draw(&hs_menu);
			menu_navigate();
			break;
	}
}