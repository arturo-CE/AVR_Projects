/*	Author: Arturo Alvarado
 *	Assignment: Lab #8  Exercise #1
 *	Exercise Description: Design a system that reads the 10-bit ADC result 
 *  from the ADC register, and displays the result on a bank of 10 LEDs.

 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif



void ADC_init(){

	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE); 
}	


int main() {
	/* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRB = 0xFF; PORTB = 0x00;

	ADC_init();
	
	unsigned short my_short;

	/* Insert your solution below */
	while (1) {
		my_short = ADC;
		unsigned char my_char = (char)(my_short);
		PORTB = my_char;
		my_char = (char)(my_short >> 8);
		PORTD = my_char;
	}
	return 0;
}
