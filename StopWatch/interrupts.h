/*
 * interrupts.h
 *
 *  Created on: Sep 30, 2020
 *      Author: Salsabeel_Essam
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/**************************************************************/
/** INTERUPT Configuration data type */
typedef enum {
	Low_Level_OR_Falling_Edge_INT2,
	Any_Logical_Change_OR_Rising_Edge_INT2,
	Falling_Edge,
	Rising_Edge
} INT_level_Edge;
typedef enum {
	INTERRUPT_2 = 5, INTERRUPT_0, INTERRUPT_1
} INT_Channel_ID;

typedef struct {
	INT_Channel_ID channelID;
	INT_level_Edge LevelEdge;
} INTERRUPT_ConfigType;

/****************************************************************/
void Interrupt_init(const INTERRUPT_ConfigType *Config_Ptr);
void Interrupt0_setCallBack(void (*a_ptr)(void));
void Interrupt1_setCallBack(void (*a_ptr)(void));
void Interrupt2_setCallBack(void (*a_ptr)(void));
#endif /* INTERRUPTS_H_ */
