
#include <stdint.h>
#include "../inc/can_controller.h"

#ifndef CAN_HANDLER_H_
#define CAN_HANDLER_H_

// Handles all can communication between Node1 and Node2
void handle_can_message(struct can_message_t *message);

#endif /* CAN_HANDLER_H_ */