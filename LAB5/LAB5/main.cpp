/*
 * LAB5.cpp
 *
 * Created: 3-1-2019 18:54:20
 * Author : jurgen
 */ 

#define F_CPU 16000000UL //16MHz
#include <util/delay.h>
#include "JR_Macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>

using namespace std;

#define FOSC F_CPU // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#define SWITCH_PRESSED !(PINB & (1<<PINB7))

void USART_Init(unsigned int ubrr);
void USART_Transmit(unsigned char data );
unsigned char USART_Receive(void);

ISR(PCINT0_vect)
{
	if(SWITCH_PRESSED)
	{
		USART_Transmit('s');
		USART_Transmit('w');
		USART_Transmit('0');
		USART_Transmit('\n');
	}
}

int main(void)
{
	PIN_X_OUTPUT(DDRB, DDB5);
	PIN_X_INPUT(DDRB, DDB7);
	
	BIT_ON_IN_REG(PCMSK0, PCINT7);
	BIT_ON_IN_REG(PCICR, PCIE0);
	
	sei();
	USART_Init(MYUBRR);
    /* Replace with your application code */
    while (1) 
    {
		unsigned char data = USART_Receive();
		USART_Transmit(data);
		if (data == 'J' || data == 'j')
		{
			LED_TOGGLE;
		}
    }
}

void USART_Init(unsigned int ubrr)
{
	/*
	Set baud rate
	*/
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	//Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/*
	Set frame format: 8data, 2stop bit
	*/
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

unsigned char USART_Receive(void)
{
/* 
Wait for data to be received
 */
while( !(UCSR0A & (1<<RXC0)) );
/* 
Get and return received data from buffer
 */
return UDR0;
}

void USART_Transmit(unsigned char data )
{
/* 
Wait for empty transmit buffer
 */
while
 ( !( UCSR0A & (1<<UDRE0)) );
/* 
Put data into buffer, sends the data
 */
UDR0 = data;
}
