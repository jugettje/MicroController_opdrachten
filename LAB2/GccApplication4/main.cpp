/*
 * LAB2.cpp
 *
 * Created: 3-1-2019 13:28:47
 * Author : jurgen
 */ 


#define F_CPU 16000000UL //16MHz
#include <util/delay.h>
#include "JR_Macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>

using namespace std;

#define SWITCH_PRESSED !(PINB & (1<<PINB7))

ISR(TIMER1_COMPA_vect)
{
	LED_TOGGLE;
}

ISR(PCINT0_vect)
{
	if(SWITCH_PRESSED)
	{
		BIT_FLIP_IN_REG(PORTB, PORTB0);
	}
}

void timer_Frequency(uint8_t freq);

int main(void)
{
	PIN_X_INPUT(DDRB, DDB7);
	PIN_X_OUTPUT(DDRB, DDB5);
	PIN_X_OUTPUT(DDRB, DDB0);
	
	BIT_ON_IN_REG(PCMSK0, PCINT7);
	BIT_ON_IN_REG(PCICR, PCIE0);
	
	timer_Frequency(2);
	
	sei();
    /* Replace with your application code */
    while (1) 
    {
		
    }
}

void timer_Frequency(uint8_t freq)
{
	/*
	*	TCCR1B (Timer \ Counter1 Control Register 1 B) 16.11.2
	*	Bit 2, 1, 0 for Clock Select
	*	We make bit 2 (CS12) high for the code (0b100) prescaler 256
	*	TCCR1B Bit 4, 3 and TCCR1A Bit 1, 0 to select the Mode of Operation 
	*	we make bit 3 of TCCR1B high for the code (0b0100) CTC mode
	*/
	BITS_2_ON_IN_REG(TCCR1B, CS12, WGM12);	
	
	/*
	*	TIMSK1 (Timer / Counter1 Interrupt Mask Register) 16.11.8
	*	Bit 1 (OCIE1A [Timer / Counter1 Output Compare A Match Interrupt Enable])
	*	Turn this bit on to trigger the interrupt if output compare A is high 
	*/
	BIT_ON_IN_REG(TIMSK1, OCIE1A);
	
		/*
	*	OCR1A (Timer / Counter 1 Output Compare Register A) 16.11.5
	*	Formula for frequency (16.9.2)
	*	[F_CPU = Fclk_I/O], [N = Prescaler]
	*	Freq = F_CPU / 2 * N * (1 + OCR1A)
	*	Change formula to calculate OCR1A with a given Freq
	*	OCR1A = F_CPU / (Freq * 2 * N) - 1
	*/
	OCR1A = (F_CPU/(freq*2*256)-1);
}