/*
 * ir_driver.c
 *
 * Created: 21.10.2020 14:47:47
 *  Author: marth
 */ 

#include "../inc/ir_driver.h"

/*
 * IR init
 * -------------------------------------------------
 * Inits ADC for analog to digital signal converting
 * Sets current and previous status for ball position
 * 1 -> in IR beam
 * 0 -> not in IR beam
*/
void ir_init(){
	adc_init();
	//prev_status_ball = 0;
	status_ball = 0;
	transmit_ball_status_flag = 1;
}


/*
 * Checks if ball is in the IR beam
 * ----------------------------------------------------------------
 * Threshold gives the lowest ADC value we can accept before
 * deciding the ball is in the beam, and current status is set to 1
 * If ADC value is above this the current status is set to 0
*/
void ir_ball_in_beam(){
	uint16_t threshold = 1000;
	if (adc_read() <= threshold ){
		status_ball = 1;
	}
	else {
		status_ball = 0;
	}
}


/*
 * Sends the current status of the ball to Node1
 * ---------------------------------------------
*/
uint8_t ir_transmit(){
	struct can_message_t ir_msg;
	ir_msg.id = 15;
	ir_msg.data[0] = status_ball;
	ir_msg.data_length = 1;
	
	uint8_t s_mailbox = 0;
	
	if(transmit_ball_status_flag == 1){
		can_send(&ir_msg, s_mailbox);		
	}	

}