/*
 * lcd.h
 *
 * Created: 5/21/2020 3:16:51 PM
 *  Author: H
 */ 


#ifndef LCD_H_
#define LCD_H_

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <util/delay.h>
#include "avr/io.h"
#include "data_types.h"
#include "bit_handle.h"

#define LCD_DATA_PORT			PORTC
#define LCD_DATA_DDR			DDRC
#define LCD_DATA_MSK			0XF0	//(0xFF for eight bits mode/0xF0 for four bits mode)

#define LCD_CTRL_PORT			PORTB
#define LCD_CTRL_DDR			DDRB

#define	LCD_RS					PB5
#define	LCD_RW					PB6		/*new with kit*/
#define	LCD_EN					PB7
/*new with kit*/
#define	LCD_BL_PORT				PORTE
#define	LCD_BL_DDR				DDRE
#define	LCD_BL_PIN				PE4
/*-------------------------------------*/
#define  CMD_CLEAR				0X01
#define  CMD_ENTRY_MODE			0X06	//Sets cursor move direction and specifies display shift.
#define  CMD_DISP_ON			0X0C	//Display on/off control
#define  CMD_EIGHT_BITS_CFG		0X38	//8_BITS_Function set
#define  CMD_FOUR_BITS_CFG		0X28	//4_BITS_Function set

#define  LCD_N_OF_DATA_BITS		0X04	//LCD mode (0x04/0x08)

/* x (1-2 line), y (1-16) cols */
void lcd_init(void);
void lcd_clear(void);
void lcd_disp_char(U_8 u8_char);
void lcd_disp_str(U_8* pu8_str);
void lcd_disp_charXY(U_8 x, U_8 y, U_8 u8_Char);
void lcd_disp_strXY(U_8 x, U_8 y, U_8* pu8_Str);
void lcd_disp_int(U_16 num);
void lcd_disp_intXY(U_8 x,U_8 y,U_16 num);
void lcd_disp_float(FP_64 num);
void lcd_disp_floatXY(U_8 x,U_8 y,FP_64 num);






#endif /* LCD_H_ */