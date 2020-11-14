/**
* @file
* @brief Library for running the actual game sequence
*/


#ifndef GAME_H_
#define GAME_H_

#include "can.h"
#include "menu.h"
#include "statemachine.h"

// Game status variables
uint16_t score_count;
uint8_t game_over;
uint8_t ir_status;


/**
* @brief Initialize game status values: score_count, game_over, ir_status
*
* @return void
*/
void game_init();

/**
* @brief Updates game over status based on input from IR on node 2
*
* @param[in] ir_status Status of ir beam in pingpong game. 0 - No blockage, 1 - Blocked ray
*
* @return void
*/
void game_over_check(uint8_t ir_status);

/**
* @brief Run game-sequence, check state and performe actions accordingly
*
* @return void
*/
void game_sequence();

#endif /* GAME_H_ */