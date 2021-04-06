/*
 * key_pad.c
 *
 * Created: 5/21/2020 12:44:13 PM
 *  Author: H
 */ 

#include "key_pad.h"

U_8 keys[4][4] = {{'7','8','9','/'},
				  {'4','5','6','*'},
				  {'1','2','3','-'},
				  {'O','0','=','+'}};
				  
void key_pad_init(void)
{
	/*ROWS OUTPUT*/
	set_bits(KEY_R_DDR,KEY_ROWS_MSK);
	/*ROWS HIGH*/
	set_bits(KEY_R_PORT,KEY_ROWS_MSK);
	/*COLS INPUT*/
	clear_bits(KEY_C_DDR,KEY_COLS_MSK);
	/*PULL_UP COLS*/
	set_bits(KEY_C_PORT,KEY_COLS_MSK);
}

U_8 key_pad_pressed_key(void)
{
	U_8 the_pressed_key = KEY_NONE;
	U_16 row=0,col=0;
	
	for (row=0;row<ROW_NUM;row++)
	{
		clear_bit(KEY_R_PORT,row);
		_delay_ms(1);
		for (col=0;col<COL_NUM;col++)
		{
			if(!read_bit(KEY_C_PIN,col+4)) 
			{
				while(!read_bit(KEY_C_PIN,col+4));
				//the_pressed_key = col + 4*row +1;	return 1 -> 16
				the_pressed_key = keys[row][col];
				return the_pressed_key;
			}
		}
		set_bit(KEY_R_PORT,row);
	}
	return KEY_NONE;
}