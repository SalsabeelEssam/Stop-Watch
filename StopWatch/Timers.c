/*
 * Timers.c
 *
 *  Created on: Oct 3, 2020
 *      Author: Salsabeel_Essam
 */
#include"timers.h"

static volatile void (*g_callBackPtr)(void) = NULL_PTR;

void Timer_init(const Timer_ConfigType *Config_Ptr) {
	/* if channal id == 0 than we use timer 0*/
	if ((Config_Ptr->channalID) == 0) {
		/* if mode == 0 then in Normal mode*/
		if ((Config_Ptr->mode) == 0) {
			TCNT0 = (uint8) (Config_Ptr->intialValue); /*Set Timer initial value to (Config_Ptr->intialValue)*/
			/* Configure the timer control register
			 * 1. Non PWM mode FOC0=1
			 * 2. Normal Mode WGM01=0 & WGM00=0
			 * 3. Normal Mode COM00=0 & COM01=0
			 * 4. clock = F_CPU/prescaler CS00=0 CS01= CS02=
			 */
			SET_BIT(TCCR0, FOC0);
			TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->clock);
			SET_BIT(TIMSK, TOIE0);/* Enable Timer0 Overflow Interrupt*/
		}
		/* if mode == 1 then in compare mode*/
		else if ((Config_Ptr->mode) == 1) {
			TCNT0 = (uint8) (Config_Ptr->intialValue);/*Set Timer initial value to (Config_Ptr->intialValue)*/
			OCR0 = (uint8) (Config_Ptr->compValue); /* Set Compare Value to (Config_Ptr->compValue) */
			/* Configure timer0 control register
			 * 1. Non PWM mode FOC0=1
			 * 2. CTC Mode WGM01=1 & WGM00=0
			 * 3. No need for OC0 in this example so COM00=0 & COM01=0
			 * 4. clock = F_CPU/prescale CS00= CS01= CS02=
			 */SET_BIT(TCCR0, FOC0);
			SET_BIT(TCCR0, WGM01);
			TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->clock);
			SET_BIT(TIMSK, OCIE0); /*Enable Timer0 Compare Interrupt*/

		}
	}/* timer 1*/
	else if ((Config_Ptr->channalID) == 1) {
		/* if mode == 0 then in Normal mode*/
		if ((Config_Ptr->mode) == 0) {
			TCNT1 = (uint16) (Config_Ptr->intialValue);/*Set Timer initial value to (Config_Ptr->intialValue)*/
			/* Configure timer1 control registers
			 * 1. Non PWM mode FOC1A=1 and FOC1B=1
			 * 2. No need for OC1A & OC1B in this example so COM1A0=0 & COM1A1=0 & COM1B0=0 & COM1B1=0
			 * 3. Normal Mode  so  WGM10=0 & WGM11=0 & WGM12=0 & WGM13=0
			 * 4. Clock = F_CPU/prescaler CS10= CS11= CS12=
			 */
			TCCR1A = (1 << FOC1A) | (1 << FOC1B);
			TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->clock);
			SET_BIT(TIMSK, TOIE1); /*enable timer1 overflow interrupt */
		}
		/* if mode == 1 then in compare mode*/
		else if ((Config_Ptr->mode) == 1) {
			TCNT1 = (uint16) (Config_Ptr->intialValue);/*Set Timer initial value to (Config_Ptr->intialValue)*/
			OCR1A = (uint16) (Config_Ptr->compValue); /* Set Compare Value to (Config_Ptr->compValue) */
			/* Configure timer1 control registers
			 * 1. Non PWM mode FOC1A=1 and FOC1B=1
			 * 2. No need for OC1A & OC1B in this example so COM1A0=0 & COM1A1=0 & COM1B0=0 & COM1B1=0
			 * 3. CTC Mode and compare value in OCR1A WGM10=0 & WGM11=0 & WGM12=1 & WGM13=0
			 * 4. Clock = F_CPU/prescaler CS10= CS11= CS12=
			 */
			TCCR1A = (1 << FOC1A) | (1 << FOC1B);
			SET_BIT(TCCR1B, WGM12);
			TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->clock);
			SET_BIT(TIMSK, OCIE1A);/*enable timer1 compare interrupt for channel A*/
		}

	}/* timer 2*/
	else if ((Config_Ptr->channalID) == 2) {
		/* if mode == 0 then in Normal mode*/
		if ((Config_Ptr->mode) == 0) {
			TCNT2 = (uint8) (Config_Ptr->intialValue); /*Set Timer initial value to (Config_Ptr->intialValue)*/
			/* Configure the timer2 control register
			 * 1. Non PWM mode FOC2=1
			 * 2. Normal Mode WGM21=0 & WGM20=0
			 * 3. Normal Mode COM20=0 & COM21=0
			 * 4. clock = F_CPU/prescaler  CS22= CS21= CS20=
			 */
			SET_BIT(TCCR2, FOC2);
			TCCR2 = (TCCR2 & 0xF8) | (Config_Ptr->clock);
			SET_BIT(TIMSK, TOIE2); /*Enable Timer2 Overflow Interrupt*/
		}
		/* if mode == 1 then in compare mode*/
		else if ((Config_Ptr->mode) == 1) {
			TCNT2 = (uint8) (Config_Ptr->intialValue); /*Set Timer initial value to (Config_Ptr->intialValue)*/
			OCR2 = (uint8) (Config_Ptr->compValue); /* Set Compare Value to (Config_Ptr->compValue) */
			/* Configure timer2 control register
			 * 1. Non PWM mode FOC2=1
			 * 2. CTC Mode WGM21=1 & WGM20=0
			 * 3. No need for OC0 in this  so COM20=0 & COM21=0
			 * 4. clock = F_CPU/prescaler CS22= CS21= CS20=
			 */
			SET_BIT(TCCR2, FOC2);
			SET_BIT(TCCR2, WGM21);
			TCCR2 = (TCCR2 & 0xF8) | (Config_Ptr->clock);
			SET_BIT(TIMSK, OCIE2); /* Enable Timer2 Compare Interrupt*/
		}

	}

}
/********************************************************************************/
void Timer_setCallBack(void (*a_ptr)(void)) {
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}
/*******************************************************************************/
void Timer_Stop(uint8 ChannelID) {
	if (ChannelID == 0) {
		TCCR0 &= 0xF8; /* No Clock  to stop timer*/
	} else if (ChannelID == 1) {
		TCCR1B &= 0xF8;
		;
	} else if (ChannelID == 2) {
		TCCR2 &= 0xF8;
		;
	}
}
/***************************************************************************/
void Timer_Set(uint8 ChannelID) {
TCNT0 = (uint8) ChannelID; /*Set Timer initial value to ChannelID */
}
/*********************************************************************/
uint8 Timer_Get(void) {
return TCNT0 ;
}

/***********************************************************************************/
void Timer_Start(uint8 channalID , uint8 clock) {
	/* clock == Config_Ptr->clock // 0 = no clock // 5 = TF_CPU_1024 */
	if (channalID == 0) {
		TCCR0 = (TCCR0 & 0xF8) | (clock & 07); /* put the same clock to resume timer*/
	} else if (channalID == 1) {
		TCCR1B = (TCCR1B & 0xF8) | (clock & 07);
	} else if (channalID == 2) {
		TCCR2 = (TCCR2 & 0xF8) |(clock & 07);
	}
}

/******************************************************************************/

/* Interrupt Service Routine for timer0 OverFlow mode */
ISR(TIMER0_OVF_vect) {
	if (g_callBackPtr != NULL_PTR) {
		/* call back function  work in timer 0 normal mode by function setCallBack */
		(*g_callBackPtr)();
		/*g_callBackPtr();*/
	}
}

/* Interrupt Service Routine for timer0 compare mode */
ISR(TIMER0_COMP_vect) {
	if (g_callBackPtr != NULL_PTR) {
		/* call back function  work in timer 0 compare mode by function setCallBack */
		(*g_callBackPtr)();
		/*g_callBackPtr();*/
	}
}

/* Interrupt Service Routine for timer1 OverFlow mode */
ISR(TIMER1_OVF_vect) {
	if (g_callBackPtr != NULL_PTR) {
		/* call back function  work in timer 1 normal mode by function setCallBack */
		(*g_callBackPtr)();
		/*g_callBackPtr();*/
	}
}

/* Interrupt Service Routine for timer1 compare mode channel A */
ISR(TIMER1_COMPA_vect) {
	if (g_callBackPtr != NULL_PTR) {
		/* call back function  work in timer 1 compare mode by function setCallBack */
		(*g_callBackPtr)();
		/*g_callBackPtr();*/
	}
}

/* Interrupt Service Routine for timer2 OverFlow mode */
ISR(TIMER2_OVF_vect) {
	if (g_callBackPtr != NULL_PTR) {
		/* call back function  work in timer 2 normal mode by function setCallBack */
		(*g_callBackPtr)();
		/*g_callBackPtr();*/
	}

}

/* Interrupt Service Routine for timer2 compare mode */
ISR(TIMER2_COMP_vect) {
	if (g_callBackPtr != NULL_PTR) {
		/* call back function  work in timer 2 compare mode by function setCallBack */
		(*g_callBackPtr)();
		/*g_callBackPtr();*/
	}

}
