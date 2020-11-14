
#ifndef GAME_H_
#define GAME_H_
#include "can.h"
#include "menu.h"
#include "statemachine.h"

uint16_t score_count;
uint8_t game_over;
uint8_t ir_status;

void game_init();

void game_over_check(uint8_t ir_status);

void game_sequence();




#endif /* GAME_H_ */