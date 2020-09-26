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
				default:
					current_state = HIGHSCORE;
					break;
			}
			break;

		case HIGHSCORE:
			switch(current_selection){
				default:
					current_state = HOME_MENU;
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
			menu_draw(&home_menu);
			break;

		case HIGHSCORE:
			menu_draw(&hs_menu);
			break;
	}
}