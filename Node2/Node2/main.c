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
    /* Initialize the SAM system */
    SystemInit();
	configure_uart();
	servo_init_pwm();
	ir_init();
	solenoid_init();
	
	// Disable watchdog
	WDT->WDT_MR = WDT_MR_WDDIS;
	motor_init();
	
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
	// can message ID 5 => ACK
	encoder_init();
	regulator_init();
    /* Replace with your application code */
    while (1)
    {
		//Checks for ball in beam
		ir_ball_in_beam();
		// Sends ball status to node 1
		
		
		//encoder_read(&encoder_value);
		//encoder_read(&encoder_value);

		//stepper_joystick_command(12);
    }
}

void TC0_Handler(){
	uint32_t dummy = REG_TC0_SR0; //Clear interrupt flag to avoid continously call to TCO_Handler()
	if (init_flag){
		init_counter += 1;
		if (init_counter >= INIT_TIME){
			regulator_finish_init();
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
