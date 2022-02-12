/*
 * interrupts.c
 *
 *  Created on: Sep 30, 2020
 *      Author: Salsabeel_Essam
 */

#include"interrupts.h"

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr0)(void) = NULL_PTR;
static volatile void (*g_callBackPtr1)(void) = NULL_PTR;
static volatile void (*g_callBackPtr2)(void) = NULL_PTR;

void Interrupt_init(const INTERRUPT_ConfigType *Config_Ptr) {
	/* if channelID == 5 then we activate interrupt2 */
	if ((Config_Ptr->channelID) == 5) {
		/* Trigger INT2 with the falling edge or RIsing edge dependent
		 * on first two number in Config_Ptr ->LevelEdge */
		MCUCSR = (MCUCSR & 0xBF) | ((Config_Ptr->LevelEdge) << 6);
		/* Configure INT2/PB2 as input pin*/

		/*Enable external interrupt pin INT2*/
		SET_BIT(GICR, INT2);
	}
	/* if channelID == 6 then we activate interrupt0 */
	else if ((Config_Ptr->channelID) == 6) {
		/* Trigger INT0 with the falling edge or rising or any change dependent on Config_Ptr ->LevelEdge */
		MCUCR = (MCUCR & 0xFC) | (Config_Ptr->LevelEdge);
		/* Configure INT0/PD2 as input pin*/
		CLEAR_BIT(DDRD, PD2);
		/* Enable external interrupt pin INT0*/
		SET_BIT(GICR, INT0);
	}
	/* if channelID == 7 then we activate interrupt1 */
	else if ((Config_Ptr->channelID) == 7) {
		/* Trigger INT0 with the falling edge or rising or any change dependent on Config_Ptr ->LevelEdge */
		MCUCR = (MCUCR & 0xF3) | ((Config_Ptr->LevelEdge) << 2);
		/* Configure INT1/PD3 as input pin*/
		CLEAR_BIT(DDRD, PD3);
		/* Enable external interrupt pin INT1*/
		SET_BIT(GICR, INT1);
	}

}
/********************************************************************************/
void Interrupt0_setCallBack(void (*a_ptr)(void)) {
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr0 = a_ptr;
}
void Interrupt1_setCallBack(void (*a_ptr)(void)) {
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr1 = a_ptr;
}
void Interrupt2_setCallBack(void (*a_ptr)(void)) {
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr2 = a_ptr;
}
/*******************************************************************************/

/* External INT0 Interrupt Service Routine */
ISR(INT0_vect) {
	if (g_callBackPtr0 != NULL_PTR) {
		/* call back function  work in INTERRUPT0  by function setCallBack */
		(*g_callBackPtr0)();
		/*g_callBackPtr();*/
	}

}
/* External INT1 Interrupt Service Routine */
ISR(INT1_vect) {
	if (g_callBackPtr1 != NULL_PTR) {
		/* call back function  work in INTERRUPT1  by function setCallBack */
		(*g_callBackPtr1)();
		/*g_callBackPtr();*/
	}

}
/* External INT2 Interrupt Service Routine */
ISR(INT2_vect) {
	if (g_callBackPtr2 != NULL_PTR) {
		/* call back function  work in INTERRUPT2  by function setCallBack */
		(*g_callBackPtr2)();
		/*g_callBackPtr();*/
	}

}
