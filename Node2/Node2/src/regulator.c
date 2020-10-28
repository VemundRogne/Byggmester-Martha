//regulator.c

#include "../inc/regulator.h"
#include "../inc/stepper.h"


void regulator_timer(){
	//Use TC instead :)
	
} 

void set_position_ref(uint8_t position){
	position_ref = position;
}



void update_motor_input(uint16_t encoder_values){
	// Calculate some PI gain, should compare encoder value with position_ref

}