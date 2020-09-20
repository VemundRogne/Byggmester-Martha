/*
 * ByggmesterMartha.c
 *
 * Created: 26/08/2020 09:09:34
 * Author : Vemund
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define F_CPU	4915200
#define BAUD	9600
#define MYUBRR	F_CPU/16/BAUD-1

#include <util/delay.h>
#include "../inc/xmem.h"
#include "../inc/uart.h"
#include "../inc/adc.h"
#include "../inc/mmi.h"
#include "../inc/oled.h"

#include <stdlib.h>

int main(void)
{
	init_UART();
	init_XMEM();
	init_adc();
	oled_init();

	init_joystick();
	
	fdevopen(&UART_tx_polling, &UART_rx_polling);	
//	fdevopen(&UART_tx_polling(char, stdout *), &UART_rx_polling(stdin *));
	
	sei();
	
	uint8_t number = 0;
	
	//SRAM_test();
	//delay_ms(1000);
	//load_mux_config();
	//_delay_ms(1000);
	//rd_adc(NULL);
	
    while (1) 
    {	

    	char bokstav = 'B';
    	printf("%d\n", bokstav);
    	char* letter = &bokstav;


    	char kielder[13] = {'B','Y','G','G','M','E','S','T','E','R','E','!','\0'};

    	oled_clear();

    	
    	char* ting = &kielder[0];
		for (uint8_t j = 0; j < 8; j++){
			oled_goto_column(0);
			oled_goto_row(j);
	    	oled_print_string("martha");
	    	ting = &kielder[0];
	    	_delay_ms(500);
	    }

		_delay_ms(1000);
		
		
		/*
		rd_adc(NULL);
		struct Joystick_pos joystick = get_joystick_pos();
		struct Slider_pos slider = get_slider_pos();
		enum Joystick_dir direction = get_joystick_dir();
		printf("%i, %i, %i\n", joystick.x, joystick.y, direction);
		*/
		
		
    }
    
}

/*
'A':

00000
11011
11011
11111
11011
11011
0111
0010




*/