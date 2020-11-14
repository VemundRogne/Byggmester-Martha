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
#include "inc/servo.h"
#include "inc/adc.h"
#include "inc/ir_driver.h"
#include "inc/motor.h"
#include "inc/delay.h"
#include "inc/regulator.h"
#include "inc/solenoid.h"

int main(void)
{
	// Disable watchdog
	WDT->WDT_MR = WDT_MR_WDDIS;
	
    /* Initialize the SAM system */
	init_flag = 2; //Some random non-zero, non-one value
    SystemInit();
	uart_configure();
	servo_init_pwm();
	ir_init();
	solenoid_init();
	motor_init();
	can_init_def_tx_rx_mb(0x00290561);
	encoder_init();
		
    while (1)
    {
		//Checks for ball in beam
		ir_ball_in_beam();

    }
}

// TC0 Interrupt handler
void TC0_Handler(){
	uint32_t dummy = REG_TC0_SR0; //Clear interrupt flag to avoid continously call to TCO_Handler()
	if (init_flag == 1){
		init_counter += 1;
		if (init_counter >= INIT_TIME){
			regulator_finish_init();
			//Tell game_play that initializing is done
			struct can_message_t ready_msg;
			ready_msg.id = 139;
			ready_msg.data_length = 1;
			ready_msg.data[0] = 1;
			
			can_send(&ready_msg, 0);
		}
	}
	else{
		regulator_run();
	}
	
	if (solenoid_free_flag == 1){
		if(solenoid_counter > 7){
			solenoid_contract();
			solenoid_counter = 0;
			solenoid_free_flag = 0;
		}
		else{
			solenoid_counter += 1;
		}
	}
}
