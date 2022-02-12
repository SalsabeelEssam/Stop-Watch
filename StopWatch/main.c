/*
 * main.c
 *
 *      Created on: Nov 5, 2020
 *      Author: Salsabeel_Essam
 */
#include"timers.h"
#include"interrupts.h"
/*flag will be set when the timer count 1 second*/
uint8 countSecondFlag = 0;
/*variables to hold the clock time*/
uint8 seconds = 0;
uint8 minutes = 0;
uint8 hours = 0;
/*****************************/
/* Prototypes call back function */
void ISRTimer1(void);
void ISRInterrupt0(void);
void ISRInterrupt1(void);
void ISRInterrupt2(void);
/************************************/
int main() {
	/*structure hold Configuration of interrupt 0 & 1 & 2*/
	INTERRUPT_ConfigType intInterrupt0 = { INTERRUPT_0, Falling_Edge };
	INTERRUPT_ConfigType intInterrupt1 = { INTERRUPT_1, Rising_Edge };
	INTERRUPT_ConfigType intInterrupt2 = { INTERRUPT_2, Falling_Edge };
	/*structure hold Configuration of timer 1 on compare mode count every 1s */
	Timer_ConfigType intTimer = { 0, 1000, TIMER1,Compare_Mode, TF_CPU_1024 };
	/* set call function */
	Timer_setCallBack(ISRTimer1);
	Interrupt0_setCallBack(ISRInterrupt0);
	Interrupt1_setCallBack(ISRInterrupt1);
	Interrupt2_setCallBack(ISRInterrupt2);
	DDRC |= 0x0F; /*configure first four pins of PORTC as output pins to led 7_segment*/
	PORTC |= 0x0F; /*initialize all the 7-segment with zero value*/
	DDRA |= 0x3F; /*configure first 6 pins in PORTA as output pins control pins*/
	PORTA &= 0xC0; /*initialize all the 7-segment with zero value control pin*/
	DDRD &= 0xF3; /* configure INT0 & INT1 as input pin*/
	PORTD |= (1<<PD2); /* enable internal pull up resistor at INT0/PD2 pin*/
	DDRB &= 0xFB; /* configure INT2/PB2 as input pin*/
	PORTB |= (1<<PB2); /* enable internal pull up resistor at INT2/PB2 pin*/
	SREG |= (1 << 7); /*enable global interrupts in MC*/
	/*start timer1 to generate compare interrupt every 1000 MiliSeconds(1 Second)*/
	Timer_init(&intTimer);
	/*activate external interrupt INT0 */
	Interrupt_init(&intInterrupt0);
	/*activate external interrupt INT01 */
	Interrupt_init(&intInterrupt1);
	/*activate external interrupt INT2 */
	Interrupt_init(&intInterrupt2);
	while (1) {
		if (countSecondFlag == 1) {
			/* enter here every one second increment seconds count */
			seconds++;
			if (seconds == 60) {
				seconds = 0;
				minutes++;
			}
			if (minutes == 60) {
				minutes = 0;
				hours++;
			}
			if (hours == 99) {
				seconds = 0;
				minutes = 0;
				hours = 0;
			}
			/*reset the flag again*/
			countSecondFlag = 0;

		} else {
			/*out the number of seconds*/
			PORTA = (PORTA & 0xC0) | 0x01;
			PORTC = (PORTC & 0xF0) | (seconds % 10);
			/* make small delay to see the changes in the 7-segment
    		 * 2Miliseconds delay will not effect the seconds count*/
			_delay_ms(2);
			PORTA = (PORTA & 0xC0) | 0x02;
			PORTC = (PORTC & 0xF0) | (seconds / 10);
			_delay_ms(2);
			PORTA = (PORTA & 0xC0) | 0x04;
			PORTC = (PORTC & 0xF0) | (minutes % 10);
			_delay_ms(2);
			PORTA = (PORTA & 0xC0) | 0x08;
			PORTC = (PORTC & 0xF0) | (minutes / 10);
			_delay_ms(2);
			PORTA = (PORTA & 0xC0) | 0x10;
			PORTC = (PORTC & 0xF0) | (hours % 10);
			_delay_ms(2);
			PORTA = (PORTA & 0xC0) | 0x20;
			PORTC = (PORTC & 0xF0) | (hours / 10);
			_delay_ms(2);
		}

	}
}
/*Interrupt Service Routine to call back function for timer1 compare mode channel A*/
void ISRTimer1(void) {
	countSecondFlag = 1;
}
/*External INT0 Interrupt Service Routine to call back function */
void ISRInterrupt0(void) {
	/* stop timer*/
	Timer_Stop(1);
	seconds =0;
	minutes =0;
	hours =0;
}
/*External INT1 Interrupt Service Routine to call back function */
void ISRInterrupt1(void) {
	/*pause timer*/
	/* Pause the stop watch by disable the timer
	 * Clear the timer clock bits (CS10=0 CS11=0 CS12=0) to stop the timer clock. */
	Timer_Start(1,0);
	/* 0 mean  TNO_CLOCK no timer clock */
}
/*External INT2 Interrupt Service Routine to call back function */
void ISRInterrupt2(void) {
	/*resume*/
	/*resume the stop watch by enable the timer through the clock bits.*/
	Timer_Start(1,5);
	/* 5 mean  TF_CPU_1024*/
}
