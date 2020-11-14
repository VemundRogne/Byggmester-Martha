/**
* @file
* @brief Library for communicating over CAN
*/

#ifndef CAN_H_
#define CAN_H_

#include <avr/io.h>

// structure of can messages
struct can_msg{
	uint16_t ID;
	uint8_t len;
	uint8_t data[8];
};

/**
* @brief Checks for valid transmition buffer
*
* @param[out] tx_buffer_address Address of first valid buffer found
*
* @return uint8_t ID of valid buffer, returns 3 if non valid buffers
*/
uint8_t can_valid_transmit_buffer(uint8_t *tx_buffer_address);

/**
* @brief Checks for pending receive buffer
*
* @param[out] tx_buffer_address Address of first pending buffer found
*
* @return uint8_t ID of valid buffer, returns 3 if non valid buffers
*/
uint8_t can_pending_receive_buffer(uint8_t *tx_buffer_address);


/**
* @brief Transmit message over can bus
*
* @param[in] msg Pointer message to be send
*
* @return uint8_t 0 on success, 1 on failure
*/
uint8_t can_transmit_message(struct can_msg *msg);

/**
* @brief Read received message from can bus
*
* @param[out] msg Points to location where received message is stored
*
* @return uint8_t 0 on success, 1 on failure
*/
uint8_t can_receive_message(struct can_msg *msg);


#endif /* CAN_H_ */