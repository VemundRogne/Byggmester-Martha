/*
 * oled.h
 *
 * Created: 19.09.2020 11:20:18
 *  Author: marth
 */ 


#ifndef OLED_H_
#define OLED_H_
#include <stdio.h>

#define OLED_COMMAND_ADDRESS 0x1000
#define OLED_DATA_ADDRESS 0x1200

void write_c(uint8_t command);

void init_oled();

//void oled_home();
//void oled_goto_line(line);
//void oled_goto_collumn(collumn);
//void oled_clear_line(line);
//void oled_pos(row, collmn);
//void oled_print(char*);


#endif /* OLED_H_ */