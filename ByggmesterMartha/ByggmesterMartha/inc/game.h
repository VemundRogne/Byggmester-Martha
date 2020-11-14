
#ifndef GAME_H_
#define GAME_H_
#include "can.h"
#include "menu.h"
#include "statemachine.h"

// Game status variables
uint16_t score_count;
uint8_t game_over;
uint8_t ir_status;

void game_init();

// Updates game over status based on input from IR on node 2
void game_over_check(uint8_t ir_status);

// Runs entire game-sequence, coninously called from main()
void game_sequence();




#endif /* GAME_H_ */