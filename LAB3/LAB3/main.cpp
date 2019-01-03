/*
 * LAB3.cpp
 *
 * Created: 3-1-2019 15:38:35
 * Author : jurgen
 */ 

#define F_CPU 16000000UL //16MHz
#include <util/delay.h>
#include "JR_Macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>

using namespace std;

ISR(TIMER0_OVF_vect)
{
	BIT_FLIP_IN_REG(PORTB, PORTB0);
}

ISR(TIMER1_OVF_vect)
{
	BIT_FLIP_IN_REG(PORTB, PORTB1);
}

ISR(TIMER2_OVF_vect)
{
	// The keyword static means that the value of the variable cnt
	// will not have changed when the ISR is executed the next time.
	static uint8_t cnt = 0;
	// Toggle PB2 after 61 count pulses
	if(cnt<31)
	{
		cnt ++;
	}
	else
	{
		cnt =0;
		PINB |= (1<< PINB2);
	}
}

int main(void)
{
	PIN_X_OUTPUT(DDRB, DDB0);
	PIN_X_OUTPUT(DDRB, DDB1);
	PIN_X_OUTPUT(DDRB, DDB2);
	
	//timer 0 in normal operation with max prescaler
	//interrupts on overflow
	//Freq = F_CPU / (2 * 1024 * 2^8) = 30Hz
	BIT_ON_IN_REG(TIMSK0, TOIE0);
	BITS_2_ON_IN_REG(TCCR0B, CS10, CS12);
	
	//timer 1 in normal operation with max prescaler
	//interrupts on overflow
	//Freq = F_CPU / (2 * 1024 * 2^16) = 0.1 Hz
	BIT_ON_IN_REG(TIMSK1, TOIE1);
	BITS_2_ON_IN_REG(TCCR1B, CS10, CS12);
	
	//timer 2 in normal operation with max prescaler
	//interrupts on overflow
	//Freq = F_CPU / (2 * 1024 * 2^8) = 30 Hz
	// (1/30) * 60 = 2 ....?
	BIT_ON_IN_REG(TIMSK2, TOIE2);
	BITS_3_ON_IN_REG(TCCR2B, CS22, CS21, CS20);
	
	sei();
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

