/*
 * Node2.cpp
 *
 * Created: 10/10/2020 11:21:35
 * Author : vemun
 */ 


#include "sam.h"
#include "inc/printf-stdarg.h"
#include "inc/can_controller.h"
#include "inc/can_interrupt.h"
#include "inc/uart.h"
#include "inc/adc.h"
#include "inc/ir_driver.h"

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	configure_uart();
	ir_init();
	
	can_init_def_tx_rx_mb(0x00290561);
	
	struct can_message_t msg;
	
	msg.id = 10;
	msg.data_length = 3;
	msg.data[0] = 0;
	msg.data[1] = 1;
	msg.data[2] = 2;
	
	REG_PIOA_OER |= (1<<19);
	
	uint8_t can_status = 1;
	// can message ID 15 => IR 
	
    /* Replace with your application code */
    while (1)
    {
		//Cheks for ball in beam
		ir_ball_in_beam();
		// Checks if status for ball has changed
		if(curr_status_ball != prev_status_ball){
			ir_transmit();
		}
		prev_status_ball = curr_status_ball;
		
		//uint16_t adc_val = adc_read();
    }
}
