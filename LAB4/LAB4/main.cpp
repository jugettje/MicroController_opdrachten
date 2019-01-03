/*
 * LAB4.cpp
 *
 * Created: 3-1-2019 17:31:08
 * Author : jurgen
 */ 

#define F_CPU 16000000UL //16MHz
#include <util/delay.h>
#include "JR_Macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>

using namespace std;

ISR(TIMER0_COMPA_vect)
{
	static uint16_t cnt = 0;
	
	if(cnt < 500)
	{
		cnt++;
	}
	else
	{
		cnt = 0;
		PINB |= (1<<PINB0);
	}
}

int main(void)
{
	BITS_6_ON_IN_REG(DDRB, DDB0, DDB1, DDB2, DDB3, DDB4, DDB5);
	
	/* set interrupt to trigger when timer = OCIE0A
	 * set operation mode to ctc
	 * set the prescaler to 64
	 * freq = F_CPU / (2 * 64 * 250 )
	 * 1/freq = 1/500 s
	 * 500 loops for 1 puls so * 500
	 * (1 / 500) * 500 = 1 Hz
	*/
	BIT_ON_IN_REG(TIMSK0, OCIE0A);
	BIT_ON_IN_REG(TCCR0A, WGM01);
	OCR0A = 249;
	BITS_2_ON_IN_REG(TCCR0B, CS10, CS11);
	
	/* set mode to ctc
	 * set prescaler to 256
	 * OCR1A = F_CPU / (Freq * 2 * 256) -1 = 31311
	 * set COM1A0 in TCCR1A to set the output directly
	*/
	BIT_ON_IN_REG(TCCR1A, COM1A0);
	OCR1A = 31311;
	BITS_2_ON_IN_REG(TCCR1B, CS12, WGM12);
	
	sei();
    /* Replace with your application code */
    while (1) 
    {
		
    }
}

