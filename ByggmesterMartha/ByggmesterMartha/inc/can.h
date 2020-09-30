#ifndef SPI_H_
#define SPI_H_



#include <stdio.h>

enum mode{
	NORMAL = 0b000,
	SLEEP = 0b001,
	LOOPBACK = 0b010,
	LISTEN = 0b011,
	CONFIG = 0b100
};


void can_init(enum mode CANmode);

void can_write();

uint8_t can_read(uint8_t address);


#endif