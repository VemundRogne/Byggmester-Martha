
#ifndef CAN_H_
#define CAN_H_

#include <avr/io.h>

// structure of can messages
struct can_msg{
	uint16_t ID;
	uint8_t len;
	uint8_t data[8];
};

// check for valid transmit buffer
uint8_t can_valid_transmit_buffer(uint8_t *tx_buffer_address);
// check for pending receive buffer
uint8_t can_pending_receive_buffer(uint8_t *rx_buffer_address);

// transmit message over can bus
uint8_t can_transmit_message(struct can_msg *msg);
// read received message from can bus
uint8_t can_receive_message(struct can_msg *msg);


#endif /* CAN_H_ */