/*
 * game.h
 *
 * Created: 28.10.2020 13:09:13
 *  Author: marth
 */ 


#ifndef GAME_H_
#define GAME_H_
#include "can.h"
#include "menu.h"
#include "statemachine.h"

volatile uint16_t score_count;
uint8_t game_over;

void game_init();

void game_score_count(uint8_t ir_status);

void game_sequence();




#endif /* GAME_H_ */