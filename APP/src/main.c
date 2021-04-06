/*
 * Microwave.c
 *
 * Created: 4/5/2021 9:11:33 PM
 * Author : Hossam Mohamed
 */ 

#include <avr/io.h>
#include "microwave.h"

int main(void)
{
	
    /* Replace with your application code */
	clear_bit(DDRD,0);
	clear_bit(DDRD,1);
	clear_bit(DDRD,2);
	clear_bit(DDRD,3);
	//clear_bit(DDRF,0);
	set_bit(PORTD,0);
	set_bit(PORTD,1);
	set_bit(PORTD,2);
	set_bit(PORTD,3);
	//set_bit(DDRF,0);
	sei();
	
	microwave_init();	
	
    while (1) 
    {
		microwave_start();
    }
}

