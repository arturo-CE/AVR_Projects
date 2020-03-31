/*	Author: Arturo Alvarado
 
 *	Assignment: Lab #8  Exercise #3
 *	Exercise Description: 
Design a system where an LED is illuminated only if enough light is 
detected from the photo resistor. Criteria: If the result of the ADC 
is >= MAX/2, the LED is illuminated.If the result of the ADC is < MAX/2, 
the LED is turned off.

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
	DDRC = 0xFF; PORTC = 0x00;

	ADC_init();
	
	unsigned short my_short;
	unsigned short MAX = 64;

	/* Insert your solution below */
	while (1) {
		my_short = ADC;
		unsigned char my_char = (char)(my_short);	
		
		if(my_char > MAX/2){
			PORTB = 0x01;
		}else{
			PORTB = 0x00;
		}
	}
	return 0;
}
