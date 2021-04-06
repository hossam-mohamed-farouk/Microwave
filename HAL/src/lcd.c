/*
 * lcd.c
 *
 * Created: 5/21/2020 3:17:03 PM
 *  Author: H
 */ 
#include "lcd.h"
#include <stdio.h>

static void lcd_send_cmd(U_8 u8_cmd);
static void lcd_send_data(U_8 u8_data);
static void lcd_gotoXY(U_8 x, U_8 y);

void lcd_init(void)
{
	/* Directions -> OUT */
	set_bits(LCD_DATA_DDR,LCD_DATA_MSK);
	set_bit(LCD_CTRL_DDR,LCD_RS);
	set_bit(LCD_CTRL_DDR,LCD_EN);
	/*RW new with kit*/
	set_bit(LCD_CTRL_DDR,LCD_RW);
	clear_bit(LCD_CTRL_PORT,LCD_RW);
	/*Back_light new with kit*/
	//set_bit(LCD_BL_DDR,LCD_BL_PIN);
	//set_bit(LCD_BL_PORT,LCD_BL_PIN);
	
	/* delay -> 20 ms (power on lcd) */
	_delay_ms(20);
	if (LCD_N_OF_DATA_BITS == 0X08)
	{
		/* 
		send CMDS:
		CMD_CFG
		CMD_DISP_ON
		CMD_ENTRY_MODE
	    */
	lcd_send_cmd(CMD_EIGHT_BITS_CFG);	 //38
	lcd_send_cmd(CMD_DISP_ON);      
	lcd_send_cmd(CMD_ENTRY_MODE);		 //06
	lcd_clear();						 //01
	}
	else if (LCD_N_OF_DATA_BITS == 0X04)
	{
		
		lcd_send_cmd(0x33);
		lcd_send_cmd(0x32);
		lcd_send_cmd(CMD_FOUR_BITS_CFG);
		
		lcd_send_cmd(CMD_DISP_ON);
		lcd_send_cmd(CMD_ENTRY_MODE);  //06
		lcd_clear();		
	}
}

void lcd_clear(void)
{
	/* CMD_CLEAR */
	lcd_send_cmd(CMD_CLEAR);
	/* delay 5 ms */
	_delay_ms(5);
}

void lcd_send_cmd(U_8 u8_cmd)
{
	if (LCD_N_OF_DATA_BITS == 0X08)
	{
		/* RS -> 0 */
		clear_bit(LCD_CTRL_PORT,LCD_RS);
		/* upload  u8Cmd*/
		LCD_DATA_PORT = u8_cmd;
		/* high and low for EN */
		set_bit(LCD_CTRL_PORT,LCD_EN);
		_delay_us(1);
		clear_bit(LCD_CTRL_PORT,LCD_EN);
		/* wait 40 us */
		_delay_us(40);
	}
	else if (LCD_N_OF_DATA_BITS == 0X04)
	{
		/* RS -> 0 */
		clear_bit(LCD_CTRL_PORT,LCD_RS);
		/* upload MSB of u8Cmd*/
		LCD_DATA_PORT = u8_cmd;// & 0XF0
		/* high and low for EN */
		set_bit(LCD_CTRL_PORT,LCD_EN);
		_delay_us(1);
		clear_bit(LCD_CTRL_PORT,LCD_EN);
		_delay_us(40);//100
		/* upload LSB of u8Cmd*/
		LCD_DATA_PORT = u8_cmd << 4;
		/* high and low for EN */
		set_bit(LCD_CTRL_PORT,LCD_EN);
		_delay_us(1);
		clear_bit(LCD_CTRL_PORT,LCD_EN);
		_delay_us(40);//100
	}
}

void lcd_send_data(U_8 u8_data)
{
	if (LCD_N_OF_DATA_BITS == 0X08)
	{
		/* RS -> 1 */
		set_bit(LCD_CTRL_PORT,LCD_RS);
		/* upload  u8Data*/
		LCD_DATA_PORT = u8_data;
		/* high and low for EN */
		set_bit(LCD_CTRL_PORT,LCD_EN);
		_delay_us(1);
		clear_bit(LCD_CTRL_PORT,LCD_EN);
		/* wait 40 us */
		_delay_us(40);
	}
	else if (LCD_N_OF_DATA_BITS == 0X04)
	{
		/* RS -> 1 */
		set_bit(LCD_CTRL_PORT,LCD_RS);
		/* upload MSB u8Data*/
		LCD_DATA_PORT = u8_data; // & 0XF0
		/* high and low for EN */
		set_bit(LCD_CTRL_PORT,LCD_EN);
		_delay_us(1);
		clear_bit(LCD_CTRL_PORT,LCD_EN);
		/* wait 40 us */
		_delay_us(100);
		/* upload LSB u8Data*/
		LCD_DATA_PORT = u8_data <<4;
		/* high and low for EN */
		set_bit(LCD_CTRL_PORT,LCD_EN);
		_delay_us(1);
		clear_bit(LCD_CTRL_PORT,LCD_EN);
		/* wait 40 us */
		_delay_us(100);
	}
}

void lcd_disp_char(U_8 u8_char)
{
	lcd_send_data(u8_char);
}
void lcd_disp_str(U_8* pu8_str)
{
	while(*pu8_str != '\0')
	{
		lcd_disp_char(*pu8_str);
		pu8_str ++;
	}
}
/*
	Line 1: 0x00
	Line 2: 0x40
*/
static void lcd_gotoXY(U_8 x, U_8 y)
{
	/* get address of DDRAM using x,y */
	if(x == 1){
		lcd_send_cmd((0x80|0x00) + (y-1)); /* Eqn */
	}
	else{
		lcd_send_cmd((0x80|0x40) + (y-1)); /* Eqn */
	}
}
void lcd_disp_charXY(U_8 x, U_8 y, U_8 u8_Char)
{
	lcd_gotoXY(x,y);
	lcd_send_data(u8_Char);
}
void lcd_disp_strXY(U_8 x, U_8 y, U_8* pu8_Str)
{
	lcd_gotoXY(x,y);
	lcd_disp_str(pu8_Str);
}

void lcd_disp_int(U_16 num)
{
	char buffer_string[17]={0};
	sprintf(buffer_string,"%d",num);
	lcd_disp_str(buffer_string);
}

void lcd_disp_intXY(U_8 x,U_8 y,U_16 num)
{
	lcd_gotoXY(x,y);
	lcd_disp_int(num);
}
/*
void lcd_disp_float(FP_64 num)
{
	char buffer_string[17]={0};
	sprintf(buffer_string,"%.3f",num);
	lcd_disp_str(buffer_string);
}
*/
void lcd_disp_floatXY(U_8 x,U_8 y,FP_64 num)
{
	lcd_gotoXY(x,y);
	lcd_disp_float(num);
}

void lcd_disp_float(FP_64 num)										/*Example number = 123.0456*/
{
	U_8		a_str[17];
	U_8*	a_tmpSign = (num < 0) ? "-" : "";
	FP_64	a_tmpVal  = (num < 0) ? -num : num;					    	/*remove the sign if negative*/
	U_16	a_tmpInt1 = a_tmpVal;								     	/* Get the integer part (123).*/
	FP_64	a_tmpFrac = a_tmpVal - a_tmpInt1;							/* Get fraction (0.0456).*/
	U_16	a_tmpInt2 = a_tmpFrac * 10000;  							/* Turn into integer (456).*/
				/* Print as parts, note that you need 0-padding for fractional bit.*/
	sprintf (a_str, "%s%d.%04d", a_tmpSign, a_tmpInt1, a_tmpInt2);
	lcd_disp_str(a_str);
}


