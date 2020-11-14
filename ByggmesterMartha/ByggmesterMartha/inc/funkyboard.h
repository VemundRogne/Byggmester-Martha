/**
* @file
* @brief Library for communicating with multifunction board
*/

#ifndef FUNKYBOARD_H_
#define FUNKYBOARD_H_

#include <avr/io.h>

// Compensation for biased joystick
int16_t joystick_offset_x;
int16_t joystick_offset_y;

// Union to send int8_t as uint8_t values over CAN
union Data {
	uint8_t u;
	int8_t i;
};
// Data association for joystick
struct Joystick_pos{
	int16_t x; 
	int16_t y;
};
enum Joystick_dir {LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3, NEUTRAL = 4};


/**
* @brief Initialize joystick with correct offset
*
* @return void
*/
void joystick_init();


/**
* @brief Read joystick position
*
* @return Joystick_pos X and Y position of joystick
*/
struct Joystick_pos get_joystick_pos();

/**
* @brief Read joystick direction
*
* @return Joystick_dir Direction of joystick
*/
enum Joystick_dir get_joystick_dir();



// Send button and joystick states to node 2
/**
* @brief Send message to node 2 if button pressed
*
* @return void
*/
void send_button_press();

/**
* @brief Send message to node 2 with joystick position
*
* @param[in] js_pos Position of joystick
*
* @return void
*/
void send_joystick_position(struct Joystick_pos js_pos);


#endif /* FUNKYBOARD_H_ */