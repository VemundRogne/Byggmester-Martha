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
void motor_enable_output();

/* Disables output from Motorbox to motor */
void motor_disable_output();

/* Sets the DIR pin based on positive or negative input */
void motor_set_direction(int8_t dir);

/* Sets the output power on the motor */
void motor_set_output(uint8_t direction, uint16_t power);

/* Initializes the motor */
void motor_init();

// ------------------ ENCODER -----------------------

/* 
 * The encoder output is a parallell interface (DO0-7), they are connected through
 * a logic level converter to PIN33-40 or PC1-PC8
*/

volatile int32_t motor_encoder_value;

/* Resets all our internal counters and stuff */
void encoder_init();

void motorbox_reset_encoder();

/* Reads the encoder value from the Motorbox */
void encoder_read(int16_t *motorbox_encoder_value);

/* Lowlevel function to get single byte from motorbox */
void _encoder_byte_read(uint8_t *_data);


#endif /* STEPPER_H_ */