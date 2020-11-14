
#ifndef CAN_H_
#define CAN_H_

#include <avr/io.h>

struct can_msg{
	uint16_t ID;
	uint8_t len;
	uint8_t data[8];
};

uint8_t can_valid_transmit_buffer(uint8_t *tx_buffer_address);
uint8_t can_pending_receive_buffer(uint8_t *rx_buffer_address);

uint8_t can_transmit_message(struct can_msg *msg);
uint8_t can_receive_message(struct can_msg *msg);


#endif /* CAN_H_ */