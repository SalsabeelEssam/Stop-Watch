/*
 * timers.h
 *
 *  Created on: Oct 3, 2020
 *      Author: Salsabeel_Essam
 */

#ifndef TIMERS_H_
#define TIMERS_H_
/*******************************************************************/
#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"
/**************************************************************/
/** ICU Configuration data type */
typedef enum {
	TNO_CLOCK,
	TF_CPU_CLOCK,
	TF_CPU_8,
	TF_CPU_64,
	TF_CPU_256,
	TF_CPU_1024,
	EXTERNAL_CLOCK_FALLING,
	EXTERNAL_CLOCK_RISING
} TIMER_Clock;

typedef enum {
	Normal_Mode, Compare_Mode
} TIMER_Mode;

typedef enum {
	TIMER0, TIMER1, TIMER2
} TIMER_Channal_ID;

typedef struct {
	uint16 intialValue;
	uint16 compValue;
	TIMER_Channal_ID channalID;
	TIMER_Mode mode;
	TIMER_Clock clock;
} Timer_ConfigType;

/****************************************************************/
void Timer_init(const Timer_ConfigType *Config_Ptr);
void Timer_setCallBack(void (*a_ptr)(void));
void Timer_Stop(uint8 ChannelID);
void Timer_Start(uint8 channalID , uint8 clock);

void Timer_Set(uint8 ChannelID);
uint8 Timer_Get(void);
#endif /* TIMERS_H_ */
