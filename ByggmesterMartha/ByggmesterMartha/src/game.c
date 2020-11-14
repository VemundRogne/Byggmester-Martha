
#define F_CPU	4915200

#include "../inc/game.h"
#include "../inc/funkyboard.h"
#include <util/delay.h>
#include <stdlib.h>

void game_init(){
	score_count = 0;
	game_over = 0;
	ir_status = 0;
}

void game_over_check(uint8_t ir_status){
	if (ir_status == 1){
		game_over = 1;
		enter_game_over(score_count);
	}
}


void game_sequence(){
	if (current_state == PLAY_GAME)
	{
		struct Joystick_pos js_pos_current = get_joystick_pos();
		//struct Slider_pos slider_pos_current = get_slider_pos();
		send_joystick_position(js_pos_current);
		//slider_can(slider_pos_current);
		send_button_press();
		score_count += 1;
	}
	
	if(current_state == HOME_MENU){
		score_count = 0;
	}
		
	statemachine_execute_current_state();
}