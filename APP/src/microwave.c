/*
 * microwave.c
 *
 * Created: 4/6/2021 2:01:28 AM
 *  Author: Hossam Mohamed
 */ 

#include "microwave.h"

static void start_btn_cb(void);
static void stop_btn_cb(void);
static void pause_btn_cb(void);
static void SLEEP_BTN_cb(void);
static void set_needed_time(void);
static void time_counter(void);
static void reset_microwave_counters(void);
static void sleep_mode(void);

static volatile microwave_status_t microwave_state = STOPPED;
static volatile int sleep_flag = SLEEP_ACTIVE;
static volatile int needed_time = 0;
static volatile int user_time[2] = {0};
static volatile int counter = 0;
static volatile int minute_count = 0;
static volatile int time_to_display = 0;
static volatile int paused_time = 0;
static volatile U_16 cooking_temp = 0;

void microwave_init(void)
{
	lcd_init();
	
	leds_init(MICROWAVE_LEDS);
	leds_off(COOKING_LED);
	leds_off(STOP_LED);
	
	temp_sens_init();
		
	key_pad_init();
	
	timer0_init(MODE_NORMAL);
	
	ext_enable(START_BTN,INT_SENSE_FALLING);
	ext_enable(STOP_BTN,INT_SENSE_FALLING);
	ext_enable(PAUSE_BTN,INT_SENSE_FALLING);
	ext_enable(SLEEP_BTN,INT_SENSE_FALLING);
	
	ext_call_setup(START_BTN,start_btn_cb);
	ext_call_setup(STOP_BTN,stop_btn_cb);
	ext_call_setup(PAUSE_BTN,pause_btn_cb);
	ext_call_setup(SLEEP_BTN,SLEEP_BTN_cb);
}

void microwave_start(void)
{
	char key = 0;
	char pressed_key = 0;
	
	if ((microwave_state == STOPPED) && (sleep_flag == SLEEP_DISACTIVE))
	{
		key = key_pad_pressed_key();
		if (key != KEY_NONE)
		{
			pressed_key = key;
			if ((pressed_key <= '9') && (pressed_key >= '0') && (counter < 2))
			{
				user_time[counter++] = pressed_key;
				lcd_disp_charXY(1,counter,pressed_key);
			}
			else
			{
				//Do no thing
			}
		} 
		else
		{
			//Do no thing
		}
	} 
	else if(microwave_state == COOKING)
	{
		leds_on(COOKING_LED);
		leds_off(STOP_LED);
		if (cooking_temp > MAX_COOKING_TEMP)
		{
			sleep_mode();
		}
	}
	else if(microwave_state == FINISHED)
	{
		lcd_clear();
		leds_on(STOP_LED);
		leds_off(COOKING_LED);
		lcd_disp_strXY(1,5,"FINISHED");
		_delay_ms(500);
		lcd_clear();
		
		reset_microwave_counters();
		
		timer0_stop();
		timer0_reset_counters();
		
		microwave_state = STOPPED;
	}
}

static void start_btn_cb(void)
{
	if ((microwave_state == STOPPED) && (sleep_flag == SLEEP_DISACTIVE))
	{
		set_needed_time();
		if (needed_time != 0)
		{
			lcd_clear();
			lcd_disp_strXY(1,1,"COOKING...");
			microwave_state = COOKING;
			timer0_set_notification(time_counter,NOTIFIER_TIME,NOTIFY_CONTINUOUS);
			lcd_disp_strXY(2,1,"Rem Time");
			lcd_disp_intXY(2,11,time_to_display);
			lcd_disp_strXY(2,14,"min");
		}
	} 
	else if(microwave_state == PAUSED)
	{
		if (needed_time != 0)
		{
			lcd_clear();
			lcd_disp_strXY(1,1,"COOKING...");
			microwave_state = COOKING;
			time_to_display = paused_time;
			needed_time = paused_time;
			lcd_disp_strXY(2,1,"Rem Time"); 
			lcd_disp_intXY(2,11,time_to_display);
			lcd_disp_strXY(2,14,"min");
			timer0_set_notification(time_counter,NOTIFIER_TIME,NOTIFY_CONTINUOUS);
		}	
	}
}

static void stop_btn_cb(void)
{
	if (sleep_flag == SLEEP_DISACTIVE)
	{
		microwave_state = STOPPED;

		leds_off(COOKING_LED);
		leds_on(STOP_LED);
		
		timer0_stop();
		timer0_reset_counters();
		
		reset_microwave_counters();
		
		lcd_clear();
	}
}

static void pause_btn_cb(void)
{
	if ((sleep_flag == SLEEP_DISACTIVE) && (microwave_state != STOPPED))
	{
		leds_off(COOKING_LED);
		leds_on(STOP_LED);
		
		lcd_clear();
		lcd_disp_strXY(1,1,"PAUSED...");
		
		microwave_state = PAUSED;
		paused_time = time_to_display;
		minute_count = 0;
		
		timer0_stop();
		timer0_reset_counters();
	}
}

static void SLEEP_BTN_cb(void)
{
	if (sleep_flag != SLEEP_DISACTIVE)
	{
		sleep_flag = SLEEP_DISACTIVE;
		leds_on(STOP_LED);
	} 
	else if(sleep_flag == SLEEP_DISACTIVE)
	{
		sleep_mode();
	}
}

static void set_needed_time(void)
{
	if (counter == 1)
	{
		needed_time = user_time[0]-48;
	} 
	else if(counter == 2)
	{
		needed_time = (user_time[0]-48)*10  + (user_time[1]-48);
	}
	else
	{
		//Do no thing
	}
	time_to_display = needed_time;
}

static void time_counter(void)
{
	minute_count ++;
	time_to_display --;
	lcd_disp_strXY(2,11,"  ");
	lcd_disp_intXY(2,11,time_to_display);
	if (minute_count == needed_time)
	{
		microwave_state = FINISHED;
	}
	cooking_temp = temperature_c(0);
}

static void reset_microwave_counters(void)
{
	minute_count = 0;
	needed_time = 0;
	counter = 0;
	user_time[0] = 0;
	user_time[1] = 0;
	paused_time = 0;
	cooking_temp = 0;
}

static void sleep_mode(void)
{
	lcd_clear();
	lcd_disp_strXY(1,5,"Sleeping...");
	_delay_ms(500);
	
	leds_off(STOP_LED);
	leds_off(COOKING_LED);
	
	microwave_state = STOPPED;
	timer0_stop();
	timer0_reset_counters();
	
	reset_microwave_counters();
	
	sleep_flag = SLEEP_ACTIVE;
	lcd_clear();
}