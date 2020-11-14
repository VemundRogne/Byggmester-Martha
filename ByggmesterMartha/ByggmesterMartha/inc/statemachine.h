/**
* @file
* @brief Library for keeping track of states and state-transitions for game play
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


/**
* @brief Initialize state to HOME_MENU
*
* @return void
*/
void statemachine_init();


/**
* @brief Transition function to HOME_MENU
*
* @return void
*/
void enter_home_menu();

/**
* @brief Transition function to DEVELOPERS
*
* @return void
*/
void enter_developers_menu();

/**
* @brief Transition function to GAME_OVER
*
* @param[in] score Final score of game play
*
* @return void
*/
void enter_game_over(uint16_t score);

/**
* @brief Transition function to PLAY_GAME
*
* @param[in] score Current score of game play
*
* @return void
*/
void enter_play_game(uint16_t score);

/**
* @brief Transition function to INITIALIZING
*
* @return void
*/
void enter_initializing();


/**
* @brief Handler users selecting a new menu
*
* @return void
*/
void statemachine_menu_selection();


/**
* @brief Execute actions according to the current state of the fsm
*
* @return void
*/
void statemachine_execute_current_state();

#endif /* STATEMACHINE_H_ */