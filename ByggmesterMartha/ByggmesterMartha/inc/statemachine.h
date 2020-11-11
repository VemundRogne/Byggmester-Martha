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
	HIGHSCORE,
	GAME_OVER,
	PLAY_GAME	
};

enum state current_state;

void init_statemachine();

void enter_home_menu();

void enter_highscore_menu();

void enter_game_over(uint16_t score);

void enter_play_game();

void statemachine_handle_menu_execute();

void statemachine_execute_current_state();

#endif /* STATEMACHINE_H_ */