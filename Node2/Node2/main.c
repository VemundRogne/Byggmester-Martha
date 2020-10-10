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
	configure_uart();
	
	REG_PIOA_OER |= (1<<19);

    /* Replace with your application code */
    while (1)
    {
		REG_PIOA_SODR |= (1<<19);
		printf("hello");
    }
}
