/*
 * microwave.h
 *
 * Created: 4/6/2021 2:01:08 AM
 *  Author: Hossam Mohamed
 */ 


#ifndef MICROWAVE_H_
#define MICROWAVE_H_

#include "data_types.h"
#include "bit_handle.h"
#include "board.h"
#include "timer.h"
#include "key_pad.h"
#include "lcd.h"
#include "ext_interrupt.h"
#include "lm_35_temp_sens.h"

#define START_BTN INT_NUM_0
#define STOP_BTN INT_NUM_1
#define PAUSE_BTN INT_NUM_2
#define SLEEP_BTN  INT_NUM_3

#define SLEEP_ACTIVE 0XFF
#define SLEEP_DISACTIVE 0X00

#define MICROWAVE_LEDS 0x03
#define STOP_LED 0x01
#define COOKING_LED 0x02

#define MAX_COOKING_TEMP 50

#define NOTIFIER_TIME 2000 //60000 FOR RIGHT TIME

typedef enum
{
	COOKING,
	STOPPED,
	PAUSED,
	FINISHED
}microwave_status_t;

void microwave_init(void);
void microwave_start(void);



#endif /* MICROWAVE_H_ */