/*
 * game.c
 *
 * Created: 28.10.2020 13:08:52
 *  Author: marth
 */ 

#include "../inc/game.h"
#include <util/delay.h>
#include <stdlib.h>

void game_init(){
	score_count = 0;
}

void game_score_count(uint8_t ir_status){
	if (ir_status == 1){
		enter_game_over(score_count);
		score_count = 0;
	}
	else {
		score_count += 1;
	}
}
