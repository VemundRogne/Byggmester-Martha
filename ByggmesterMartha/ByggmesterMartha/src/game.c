/*
 * game.c
 *
 * Created: 28.10.2020 13:08:52
 *  Author: marth
 */ 

#include "../inc/game.h"
#include "../inc/funkyboard.h"
#include <util/delay.h>
#include <stdlib.h>

void game_init(){
	score_count = 0;
	game_over = 0;
}

void game_score_count(uint8_t ir_status){
	if (ir_status == 1){
		game_over = 1;
		enter_game_over(score_count);
	}
	//else {
		//if(game_over == 0){
		//	score_count += 1;
		//}
	//}
}


void game_sequence(){
	
	if (current_state == PLAY_GAME)
	{
		struct Joystick_pos js_pos_current = get_joystick_pos();
		struct Slider_pos slider_pos_current = get_slider_pos();
		joystick_transmit_position(js_pos_current);
		//slider_can(slider_pos_current);
		send_button_press();
		score_count += 1;
	}
	
	if(current_state == HOME_MENU){
		score_count = 0;
	}
		
	statemachine_execute_current_state();
	
		
}