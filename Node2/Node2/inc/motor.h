/*
 * motor.h
 * 
 * Lowlevel functions to control the motor
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>

// MJ1 output from shield
// DIR 		-> PIN32 	-> PD10
// EN 		-> PIN30 	-> PD9
// SEL 		-> PIN27 	-> PD2
// NOT_RST	-> PIN26 	-> PD1
// NOT_OE	-> PIN25	-> PD0
#define DIR         10
#define EN          9
#define SEL         2
#define RST         1
#define OE          0
#define DACC_PIO    16

// ------------------ MOTOR -----------------------
/* Enables output from Motorbox to motor */
void motor_enable();

/* Disables output from Motorbox to motor */
void motor_disable();

/* Sets the DIR pin based on positive or negative input */
void motor_set_direction(int8_t dir);

/* Sets the output power on the motor */
void motor_set_output(uint8_t direction, uint16_t power);

/* Initializes the motor */
void motor_init();


// Stepper speed should be commanded by joystick (-127, 127)
//void motor_joystick_command(int8_t stepper_speed);


// ------------------ ENCODER -----------------------



#endif /* STEPPER_H_ */