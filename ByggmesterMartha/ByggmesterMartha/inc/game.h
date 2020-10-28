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

uint8_t score_count;

void game_init();

void game_score_count(uint8_t ir_status);





#endif /* GAME_H_ */