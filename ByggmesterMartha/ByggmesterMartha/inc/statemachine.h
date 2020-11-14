/*
 * statemachine.h
 *
 * Created: 26/09/2020 13:43:32
 *  Author: 
 */ 


#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

enum state{
	HOME_MENU,
	DEVELOPERS,
	GAME_OVER,
	PLAY_GAME,
	INITIALIZING, 
	END_GAME	
};
enum state current_state;


void init_statemachine();


// STATE ENTRY FUNCTIONS //
void enter_home_menu();
void enter_developers_menu();
void enter_game_over(uint16_t score);
void enter_play_game(uint16_t score);
void enter_initializing();


// HANDLE MENU SELECTIONS //
void statemachine_menu_selection();


// FSM EXECUTION //
void statemachine_execute_current_state();

#endif /* STATEMACHINE_H_ */