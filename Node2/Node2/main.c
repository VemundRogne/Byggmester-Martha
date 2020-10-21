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

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	//configure_uart();
	
	REG_PIOA_OER |= (1<<19);
	uint8_t can_status = can_init_def_tx_rx_mb(construct_can_br_register());
	
	
	if(can_status == 0){
		//REG_PIOA_SODR |= (1<<19);
	}
	
	struct can_message_t message;
	message.id = 10;
	message.data_length = 2;
	message.data[0] = 1;
	message.data[1] = 2;

    /* Replace with your application code */
    while (1)
    {
		can_status = can_send(&message, 0);
		if(can_status == 0){
			REG_PIOA_SODR |= (1<<19);
		}
		else{
			REG_PIOA_CODR |= (1<<19);
		}
    }
}
